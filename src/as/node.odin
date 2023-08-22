package as

word :: u16

Node_Type :: enum {
    Raw_Data,
    Alias,
    Label,
    Instruction
}

Raw_Data_Node :: struct {
    length : int,
    as : union {
        Identifier,
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
    Identifier,
    word,
}

Instr_Node :: struct {
    instr : Instruction,
    op_type : Token_Type,
    operand : union {
        Identifier,
        byte,
        word,
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