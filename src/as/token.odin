package as

Token_Type :: enum {
    Colon,
    Equal,
    Directive,
    Instruction,
    Register_X,
    Register_Y,
    Imm_Literal,
    Imm_Identifier,
    Stk_Literal,
    Abs_Literal,
    Abs_Identifier,
    String,
    Eof
}

Directive :: enum {
    Byte,
    Word,
    String
}

Instruction :: enum {
    NOP,
    
    LDA,
    LDX,
    LDY,
    LDI,
    STA,
    STX,
    STY,
    STI,

    TAX,
    TAY,
    TXA,
    TYA,
    SAX,
    SAY,
    SXY,

    TSX,
    TXS,
    PSH,
    PLL,
    POP,

    AND,
    IOR,
    XOR,
    SHL,
    SHR,
    NOT,

    ADD,
    SUB,
    MUL,
    DIV,
    INC,
    DEC,
    CMP,

    JMP,
    JZS,
    JZN,
    JCS,
    JCN,
    JES,
    JEN,
    JLS,
    JLN,
    JGS,
    JGN,
    JSR,
    RTS,
    RTI,
    HLT,
}

Imm_Literal :: byte
Abs_Literal :: word

Token_As :: union {
    Directive,
    Instruction,
    Imm_Literal,
    Abs_Literal,
    string
}

Token :: struct {
    line, length : int,
    type : Token_Type,
    as : Token_As,
}