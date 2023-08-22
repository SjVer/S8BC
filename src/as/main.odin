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
        log.error("could not read", args.asm_file)
        os.exit(1)
    }
    src := string(data)

    // init stuff
    init_lexer(src)
    init_parser()

    nodes := parse()
}