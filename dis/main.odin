package dis

import "core:runtime"
import "core:log"
import "core:os"

main :: proc() {
    context.logger = log.create_console_logger(
        opt = {.Level},
        ident = "s8bc-dis"
    )

    // check arguments
    if len(os.args) <= 1 {
        log.error("invalid usage")
        log.infof("usage: %v <ROM files>", os.args[0])
    }

    // disassemble files
    for f in os.args[1:] {
        disassemble(f)
    }
}