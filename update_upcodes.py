import re

SRC_FILE = "src/spec/instructions.odin"
OPCODE_REGEX = r"(\s+)(\w+)(\s+)= 0x.*,(.*)"

opcodes = {}

def transform():
    with open(SRC_FILE, 'r') as f:
        old_lines = f.read().split('\n')
    new_lines = []

    c = 0x00
    for l in old_lines:
        if m := re.search(OPCODE_REGEX, l):
            opcodes[m.group(2)] = c
            nl = "{}{}{}= 0x{:02x},{}".format(
                m.group(1), m.group(2), m.group(3), c, m.group(4))
            c += 1
            new_lines.append(nl)
        else:
            new_lines.append(l)

    with open(SRC_FILE, 'w') as f:
        f.write('\n'.join(new_lines))

if __name__ == "__main__":
    transform()