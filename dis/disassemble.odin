package dis

import "core:log"
import "core:os"
import "core:fmt"

disassemble :: proc(path: string) {
    log.debug("disassembling", path)

    // read file
    data, success := os.read_entire_file_from_filename(path)
    if !success {
        log.error("could not open", path);
        return
    }

    // check data
    if len(data) != 0x8000 {
        log.warnf("file contains 0x%x bytes, instead of 0x8000", len(data))
    }

    // decode instructions
    decode(data)
}

decode :: proc(data : []byte) {
    for i := 0; i < len(data); i += 1 {
        pc := i

        if data[i] == 0 { 
            // skip nops
            for ; i < len(data) && data[i] == 0; i += 1 {}
            
            if i - pc >= 3 {
                fmt.printf("$%04x-$%04x: nop\n", pc, i)
                pc = i
            } else {
                i = pc
            }
        }
        
        instr := instructions[data[i]] or_else Instruction {
            name = fmt.aprintf("#$%02x", data[i]),
            mode = .None,
            arg = nil
        }
        
        switch instr.mode {
            case .None:
                break
            case .Immediate:
                i += 1
                instr.arg = data[i]
                break
            case .Implied:
                break
            case .OperandX:
                break
            case .OperandY:
                break
            case .Stack:
                i += 1
                instr.arg = data[i]
                break
            case .Absolute:
                i += 2
                instr.arg = (u16)(data[i] << 8 | data[i - 1])
                break
        }

        fmt.printf("$%04x: ", pc + 0x8000)
        fmt.println(instr_to_string(instr))
    }
}