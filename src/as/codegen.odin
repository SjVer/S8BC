package as

import "../spec"

c_rom : []byte

generate_rom :: proc(nodes: [dynamic]Node) -> []byte {
    c_rom = make([]byte, spec.ROM_SIZE)

    return c_rom
}