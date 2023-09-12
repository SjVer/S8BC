package as

import "../spec"
import "core:os"
import "core:log"
import "core:fmt"
import "core:runtime"

args : struct {
    asm_file : string,
    out_file : string,
    verbose : bool,
}

abort :: proc(fmt : string, args : ..any) -> ! {
    log.errorf(fmt, ..args)
    os.exit(1)
}

main :: proc() {
    context.logger = log.create_console_logger(
        opt = {.Terminal_Color, .Level, .Short_File_Path, .Line}
    )

    args.asm_file = "test/test.s"
    args.out_file = "bin/test.rom"

    // read file
    data, success := os.read_entire_file_from_filename(args.asm_file)
    if !success {
        abort("could not read %s", args.asm_file)
    }
    src := string(data)

    // init stuff
    init_lexer(src)
    init_parser()

    // parse and solve addresses
    nodes := parse()
    solve_addresses(nodes)

    for node in nodes {
        log.debugf("$%04x: %v", node.address, node.as)
    }

    // codegen
    rom := generate_rom(nodes)

    // write to file
    if !os.write_entire_file(args.out_file, rom) {
        abort("could not write %s", args.out_file)
    }
}