from re import match
from dataclasses import dataclass

SIG_FILE = "../docs/SIGNALS.md"
SIG_START_1 = "<!-- SIG_START -->"
SIG_START_2 = "| --- | --- | --- | --- |\n"
SIG_END = "<!-- SIG_END -->"
SIG_REGEX = r"^\|\s*\d+\s*\|(.*)\|\s*(\d+)\s*\|\s*(.*)$"
SIG_FMT = "| {bit} | {name} | {width} | {comment}\n"

CODE_FILE = "../docs/MICROCODE.md"
CODE_START_1 = "<!-- CODE_START -->"
CODE_START_2 = "| --- | --- | --- | --- |\n"
CODE_END = "<!-- CODE_END -->"
CODE_REGEX = r"^\|\s*(?:\$(..))?\s*\|(?:\s*`(.*)`(?: (.+))?)?\s*\|\s*(\d+)\s*\|\s*`(.*)`\s*$"

FIRST_STEP = "PC/AB, AB/DP, Mem/DB, DB/IR; PC-En"
STEP_COUNT = 7

ROM_FILE = "csd_rom.txt"
ROM_START = "v2.0 raw\n"

@dataclass
class Sig:
    name: str
    width: int
    comment: str

@dataclass
class Op:
    name: str
    steps: list[list[Sig]]

signals : dict[int, Sig] = {}
ops : dict[int, Op] = {}

def get_signals():
    with open(SIG_FILE, "r") as f:
        src = f.read() \
            .split(SIG_START_1)[1] \
            .split(SIG_START_2)[1] \
            .split(SIG_END)[0]
    
    bit = 0
    for l in [l for l in src.splitlines() if l.strip()]:
        m = match(SIG_REGEX, l)
        if m is None: continue

        signals[bit] = Sig(
            m[1].strip(), 
            eval(m[2]), m[3].strip()
        )
        bit += eval(m[2])

def edit_signals_md():
    with open(SIG_FILE, "r") as f:
        old = f.read()

    parts = old.partition(SIG_START_1)
    new = parts[0] + parts[1]
    parts = parts[2].partition(SIG_START_2)
    new += parts[0] + parts[1]
    parts = parts[2].partition(SIG_END)
    
    for bit, s in signals.items():
        new += SIG_FMT.format(
            bit = str(bit).ljust(2),
            name = s.name.ljust(8),
            width = s.width,
            comment = s.comment
        )

    new += parts[1] + parts[2]

    with open(SIG_FILE, "w") as f:
        f.write(new)

def get_code():
    with open(CODE_FILE, "r") as f:
        src = f.read() \
            .split(CODE_START_1)[1] \
            .split(CODE_START_2)[1] \
            .split(CODE_END)[0]

    curr_op = None
    curr_opcode = None
        
    for l in [l for l in src.splitlines() if l.strip()]:
        m = match(CODE_REGEX, l)
        if m is None: continue

        [opcode, name, mode, step, sigs] = m.groups()

        if opcode: # start of new op
            ops[curr_opcode] = curr_op

            if mode.strip(): name += " " + mode.strip()

            curr_opcode = eval("0x" + opcode)
            curr_op = Op(name, [None for _ in range(STEP_COUNT)])
            curr_op.steps[0] = FIRST_STEP
            curr_op.steps[1] = sigs

        else:
            curr_op.steps[eval(m[4]) - 1] = m[5]

    ops[curr_opcode] = curr_op
    ops.pop(None)

def main():
    get_signals()
    edit_signals_md()
    get_code()
    for op in ops: print(op, ops[op])

if __name__ == "__main__":
    main()  