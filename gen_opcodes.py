import re

H_SRC_FILE = "include/instructions.h"
OPCODE_REGEX = r"(\s+)OP_(\w+)(\s+)= 0x.*,(.*)"

MD_SRC_FILE = "docs/OPCODES.md"
MD_START_REGEX = "| --- | --- | --- |"
MD_END_REGEX = "<!-- END OPCODES -->"
MD_FORMAT = "| ${:02X} | `{}` | {} |\n"

opcodes = {}
modes = {
    "": "",
    "IMM": "immediate",
    "IMP": "implied",
    "OPX": "operand X",
    "OPY": "operand Y",
    "ABS": "absolute",
}

def transform_h():
    with open(H_SRC_FILE, 'r') as f:
        old_lines = f.read().split('\n')
    new_lines = []

    c = 0x00
    for l in old_lines:
        if m := re.search(OPCODE_REGEX, l):
            opcodes[m.group(2)] = c
            nl = "{}OP_{}{}= 0x{:02x},{}".format(
                m.group(1), m.group(2), m.group(3), c, m.group(4))
            c += 1
            new_lines.append(nl)
        else:
            new_lines.append(l)

    with open(H_SRC_FILE, 'w') as f:
        f.write('\n'.join(new_lines))

def transform_md():
    with open(MD_SRC_FILE, 'r') as f:
        src = f.read()

    before, split1, middle = src.partition(MD_START_REGEX)
    middle, split2, after = middle.partition(MD_END_REGEX)
    
    new_middle = ""
    for key, code in opcodes.items():
        name, _, mode = key.partition('_')
        new_middle += MD_FORMAT.format(code, name.lower(), modes[mode])

    with open(MD_SRC_FILE, 'w') as f:
        f.write(f"{before}{split1}\n{new_middle}{split2}{after}")

if __name__ == "__main__":
    transform_h()
    transform_md()
