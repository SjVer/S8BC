package dis

import "core:fmt"

Instr_Mode :: enum {
    None,
    Immediate,
    Implied,
    OperandX,
    OperandY,
    Stack,
    Absolute
}

Reg :: enum { X, Y }

Instr_Arg :: union {
    byte,
    Reg,
    u16
}

Instruction :: struct {
    name: string,
    mode: Instr_Mode,
    arg: Instr_Arg
}

check_arg :: proc(i: Instruction) -> bool {
    switch i.mode {
        case .None:
            return i.arg == nil
        case .Immediate:
            _, ok := i.arg.(byte)
            return ok
        case .Implied:
            return i.arg == nil
        case .OperandX:
            return i.arg.(Reg) == .X
        case .OperandY:
            return i.arg.(Reg) == .Y
        case .Stack:
            _, ok := i.arg.(byte)
            return ok
        case .Absolute:
            _, ok := i.arg.(u16)
            return ok
    }

    return false;
}

instr_to_string :: proc(i: Instruction) -> string {
    switch i.mode {
        case .None:
            return i.name
        case .Immediate:
            return fmt.aprintf("%v #$%x", i.name, i.arg.(byte))
        case .Implied:
            return i.name
        case .OperandX:
            return fmt.aprint(i.name, "x")
        case .OperandY:
            return fmt.aprint(i.name, "y")
        case .Stack:
            return fmt.aprintf("%v @%v", i.name, i.arg.(byte))
        case .Absolute:
            return fmt.aprintf("%v $%04x", i.name, i.arg.(u16))
    }

    return "<INVALID INSTRUCTION MODE>"
}