from re import match
from dataclasses import dataclass
from json import dumps

SIG_FILE = "../docs/SIGNALS.md"
SIG_START_1 = "<!-- SIG_START -->"
SIG_START_2 = "| --- | --- | --- | --- |\n"
SIG_END = "<!-- SIG_END -->"
SIG_REGEX = r"^\|\s*\d+\s*\|(.*)\|\s*(\d+)\s*\|\s*(.*)$"
SIG_FMT = "| {bit} | {name} | {width} | {comment}\n"

CODE_FILE = "../docs/MICROCODE.md"
ALIAS_START_1 = "<!-- ALIAS_START -->"
ALIAS_START_2 = "| --- | --- |\n"
ALIAS_REGEX = r"^\|\s*`(.*)`\s*\|\s*`(.*)`\s*$"
ALIAS_END = "<!-- ALIAS_END -->"
CODE_START_1 = "<!-- CODE_START -->"
CODE_START_2 = "| --- | --- | --- | --- |\n"
CODE_END = "<!-- CODE_END -->"
CODE_REGEX = r"^\|\s*(?:\$(..))?\s*\|(?:\s*`(.*)`(?: (.+))?)?\s*\|\s*(\d+)\s*\|\s*`(.*)`\s*$"

FIRST_STEP = "Fetch->DB, DB/IR"
STEP_COUNT = 8

ROM_FILE = "csd_rom.txt"
ROM_START = "v3.0 hex words addressed\n"
ROM_FTM = "{address:03x}: {signals:011x}\n"

@dataclass
class Sig:
    bit: int
    width: int
    comment: str

@dataclass
class Op:
    name: str
    steps: list[list[Sig]]

signals : dict[str, Sig] = {}
aliases : dict[str, str] = {}
ops : dict[int, Op] = {}

def get_signals():
    with open(SIG_FILE, "r") as f:
        src = f.read() \
            .split(SIG_START_1)[1] \
            .split(SIG_START_2)[1] \
            .split(SIG_END)[0]
    
    bit = 0
    for l in src.splitlines():
        m = match(SIG_REGEX, l)
        if m is None: continue

        signals[m[1].strip()] = Sig(
            bit, eval(m[2]), m[3].strip())
        bit += eval(m[2])

def edit_signals_md():
    with open(SIG_FILE, "r") as f:
        old = f.read()

    parts = old.partition(SIG_START_1)
    new = parts[0] + parts[1]
    parts = parts[2].partition(SIG_START_2)
    new += parts[0] + parts[1]
    parts = parts[2].partition(SIG_END)
  
    for name, sig in signals.items():
        new += SIG_FMT.format(
            bit = str(sig.bit).ljust(2),
            name = name.ljust(8),
            width = sig.width,
            comment = sig.comment
        )

    new += parts[1] + parts[2]

    with open(SIG_FILE, "w") as f:
        f.write(new)

def get_aliases():
    with open(CODE_FILE, "r") as f:
        src = f.read() \
            .split(ALIAS_START_1)[1] \
            .split(ALIAS_START_2)[1] \
            .split(ALIAS_END)[0]

    for l in src.splitlines():
        m = match(ALIAS_REGEX, l)
        if m is None: continue
        aliases[m[1]] = m[2]

def parse_sigs(txt: str):
    if not txt.strip(): return []

    names = [n.strip() for n in txt.split(",")]

    sigs = []
    for name in names:
        try: sigs.append(signals[name])
        except KeyError:
            try:
                txt = aliases[name]
                sigs += parse_sigs(txt)
            except KeyError:
                print(f"INVALID SIGNAL: '{name}' in `{txt}`")

    return sigs

def get_microcode():
    with open(CODE_FILE, "r") as f:
        src = f.read() \
            .split(CODE_START_1)[1] \
            .split(CODE_START_2)[1] \
            .split(CODE_END)[0]

    curr_op = None
    curr_opcode = None
        
    for l in src.splitlines():
        m = match(CODE_REGEX, l)
        if m is None: continue

        [opcode, name, mode, step, sigs] = m.groups()

        if opcode: # start of new op
            ops[curr_opcode] = curr_op

            if mode.strip(): name += " " + mode.strip()

            curr_opcode = eval("0x" + opcode)
            curr_op = Op(name, [None for _ in range(STEP_COUNT)])
            curr_op.steps[0] = parse_sigs(FIRST_STEP)
            curr_op.steps[eval(step) - 1] = parse_sigs(sigs)

        else:
            curr_op.steps[eval(step) - 1] = parse_sigs(m[5])

    ops[curr_opcode] = curr_op
    ops.pop(None)

def encode_sigs(sigs: list[Sig]):
    word = 0

    for sig in sigs:
        if sig.width == 1:
            word |= 1 << sig.bit

    return word

def write_rom():
    txt = ROM_START

    for opcode, op in ops.items():
        for step, signals in zip(range(len(op.steps)), op.steps):
            txt += ROM_FTM.format(
                address = opcode << 3 | step,
                signals = encode_sigs(signals) if signals else 0
            )

    with open(ROM_FILE, "w") as f:
        f.write(txt)

def main():
    get_signals()
    edit_signals_md()
    
    get_aliases()
    get_microcode()

    write_rom()

if __name__ == "__main__":
    main()  