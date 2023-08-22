package spec

OperandMode :: enum {
    Imm, // immediate (literal byte)
    Imp, // implied (byte in A)
    OpX, // operand X (byte in/at X)
    OpY, // operand Y (byte in/at Y)
    Stk, // stack (n'th item in stack)
    Abs, // absolute (byte at literal address)
}

OpCode :: enum {
    NOP     = 0x00, // no operation

    // load/store operations

    LDA_Imm = 0x01, // load OP into A
    LDA_OpX = 0x02,
    LDA_OpY = 0x03,
    LDA_Stk = 0x04,
    LDA_Abs = 0x05,

    LDX_Imm = 0x06, // load OP into X
    LDX_OpY = 0x07,
    LDX_Stk = 0x08,
    LDX_Abs = 0x09,

    LDY_Imm = 0x0a, // load OP into Y
    LDY_OpX = 0x0b,
    LDY_Stk = 0x0c,
    LDY_Abs = 0x0d,

    LDI_Abs = 0x0e, // load byte at address at OP into A

    STA_OpX = 0x0f, // store A at OP
    STA_OpY = 0x10,
    STA_Stk = 0x11,
    STA_Abs = 0x12,

    STX_Abs = 0x13, // store X at OP
    STY_Abs = 0x14, // store Y at OP
    STI_Abs = 0x15, // store A at address at OP

    // register operations

    TAX     = 0x16, // transfer A to X
    TAY     = 0x17, // transfer A to Y
    TXA     = 0x18, // transfer X to A
    TYA     = 0x19, // transfer Y to A
    SAX     = 0x1a, // swap A and X
    SAY     = 0x1b, // swap A and Y
    SXY     = 0x1c, // swap X and Y

    // stack operations
    
    TSX     = 0x1d, // transfer SP to X
    TXS     = 0x1e, // transfer X to SP

    PSH_Imm = 0x1f, // push OP on stack
    PSH_Imp = 0x20,
    PSH_OpX = 0x21,
    PSH_OpY = 0x22,
    
    PLL     = 0x23, // pull stack to A

    PIMP    = 0x24, // pop stack to OP
    POPX    = 0x25,
    POPY    = 0x26,

    // bitwise operations

    AND_Imm = 0x27, // A & OP -> A
    AND_OpX = 0x28,
    AND_Abs = 0x29,

    IOR_Imm = 0x2a, // A | OP -> A
    IOR_OpX = 0x2b,
    IOR_Abs = 0x2c,

    XOR_Imm = 0x2d, // A ^ OP -> A
    XOR_OpX = 0x2e,
    XOR_Abs = 0x2f,

    SHL_Imm = 0x30, // A << OP -> A
    SHL_OpX = 0x31,
    SHL_Abs = 0x32,

    SHR_Imm = 0x33, // A >> OP -> A
    SHR_OpX = 0x34,
    SHR_Abs = 0x35,

    NOT_Imp = 0x36, // bitwise negate OP
    NOT_OpX = 0x37,
    NOT_OpY = 0x38,
    NOT_Abs = 0x39,

    // numerical operations

    ADD_Imm = 0x3a, // A + OP -> A
    ADD_OpX = 0x3b,
    ADD_Abs = 0x3c,

    SUB_Imm = 0x3d, // A - OP -> A
    SUB_OpX = 0x3e,
    SUB_Abs = 0x3f,

    MUL_Imm = 0x40, // A * OP -> A
    MUL_OpX = 0x41,
    MUL_Abs = 0x42,
    
    DIV_Imm = 0x43, // A / OP -> A
    DIV_OpX = 0x44,
    DIV_Abs = 0x45,

    INC_Imp = 0x46, // increment OP
    INC_OpX = 0x47,
    INC_OpY = 0x48,
    INC_Abs = 0x49,

    DEC_Imp = 0x4a, // decrement OP
    DEC_OpX = 0x4b,
    DEC_OpY = 0x4c,
    DEC_Abs = 0x4d,

    CMP_Imm = 0x4e, // compare A and OP
    CMP_OpX = 0x4f,
    CMP_OpY = 0x50,
    CMP_Abs = 0x51,

    // control flow operations

    JMP     = 0x52, // jump to OP
    JZS     = 0x53, // jump to OP if zero flag set
    JZN     = 0x54, // jump to OP if zero flag not set
    JCS     = 0x55, // jump to OP if carry flag set
    JCN     = 0x56, // jump to OP if carry flag not set
    JES     = 0x57, // jump to OP if equal flag set
    JEN     = 0x58, // jump to OP if equal flag not set
    JLS     = 0x59, // jump to OP if less than flag set
    JLN     = 0x5a, // jump to OP if less than flag not set
    JGS     = 0x5b, // jump to OP if greater flag set
    JGN     = 0x5c, // jump to OP if greater flag not set
    
    JSR     = 0x5d, // call a subroutine at OP
    RTS     = 0x5e, // return from a subroutine
    RTI     = 0x5f, // return from an interrupt
    HLT     = 0x60, // halt execution

}