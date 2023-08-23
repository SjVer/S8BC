package as

import "../spec"

word :: u16

Node_Type :: enum {
    Raw_Data,
    Alias,
    Label,
    Instruction
}

Raw_Data_Node :: struct {
    length : uint,
    as : union {
        string,
        byte,
        word,
        []byte
    }
}

Alias_Node :: struct {
    name : string,
    address: word,
}

Label_Node :: union {
    string,
    word,
}

Instr_Node :: struct {
    instr : Instruction,
    op_mode : spec.OperandMode,
    operand : union {
        string,
        Imm_Literal,
        Abs_Literal,
    }
}

Node :: struct {
    address : word,
    type : Node_Type,
    as : union {
        Raw_Data_Node,
        Alias_Node,
        Label_Node,
        Instr_Node,
    }
}