from re import match
from dataclasses import dataclass

SIG_FILE = "../docs/SIGNALS.md"
SIG_START_1 = "<!-- SIG_START -->"
SIG_START_2 = "| --- | --- | --- | --- |\n"
SIG_END = "<!-- SIG_END -->"
SIG_REGEX = r"^\|\s*\d+\s*\|(.*)\|\s*(\d+)\s*\|\s*(.*)$"
SIG_FMT = "| {bit} | {name} | {width} | {comment}\n"

ROM_FILE = "csd_rom.txt"
ROM_START = "v2.0 raw\n"

@dataclass
class Sig:
    bit: int
    name: str
    width: int
    comment: str

@dataclass
class Op:
    code: int
    name: str
    steps: list[list[Sig]]

signals : list[Sig] = []

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

        signals.append(Sig(
            bit, m[1].strip(), 
            eval(m[2]), m[3].strip()
        ))
        bit += eval(m[2])

def edit_signals_md():
    with open(SIG_FILE, "r") as f:
        old = f.read()

    parts = old.partition(SIG_START_1)
    new = parts[0] + parts[1]
    parts = parts[2].partition(SIG_START_2)
    new += parts[0] + parts[1]
    parts = parts[2].partition(SIG_END)
    
    for s in signals:
        new += SIG_FMT.format(
            bit = str(s.bit).ljust(2),
            name = s.name.ljust(8),
            width = s.width,
            comment = s.comment
        )

    new += parts[1] + parts[2]

    with open(SIG_FILE, "w") as f:
        f.write(new)

def main():
    get_signals()
    edit_signals_md()

if __name__ == "__main__":
    main()  