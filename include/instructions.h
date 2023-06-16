#pragma once
#define INSTRUCTIONS_H

#include "common.h"

/*
  definitions:
    A: the accumulator (A register)
    X: the X register
    Y: the Y register
    PC: the program counter
    SP: the stack pointer
    
    OP: the operand

  operand modes:
    IMM: immediate (literal byte)
    IMP: implied (byte in A)
    OPX: operand X (byte in/at X)
    OPY: operand Y (byte in/at Y)
    ABS: absolute (byte at literal address)
*/

typedef enum {
    OP_NOP     = 0x00, // no operation

    // load/store operations

    OP_LDA_IMM = 0x01, // load OP into A
    OP_LDA_OPX = 0x02,
    OP_LDA_OPY = 0x03,
    OP_LDA_ABS = 0x04,

    OP_LDX_IMM = 0x05, // load OP into X
    OP_LDX_OPY = 0x06,
    OP_LDX_ABS = 0x07,

    OP_LDY_IMM = 0x08, // load OP into Y
    OP_LDY_OPX = 0x09,
    OP_LDY_ABS = 0x0a,

    OP_LDI_ABS = 0x0b, // load byte at address at OP into A

    OP_STA_OPX = 0x0c, // store A at OP
    OP_STA_OPY = 0x0d,
    OP_STA_ABS = 0x0e,

    OP_STX_ABS = 0x0f, // store X at OP
    OP_STY_ABS = 0x10, // store Y at OP
    OP_STI_ABS = 0x11, // store A at address at OP

    // register operations

    OP_TAX     = 0x12, // transfer A to X
    OP_TAY     = 0x13, // transfer A to Y
    OP_TXA     = 0x14, // transfer X to A
    OP_TYA     = 0x15, // transfer Y to A
    OP_SAX     = 0x16, // swap A and X
    OP_SAY     = 0x17, // swap A and Y
    OP_SXY     = 0x18, // swap X and Y

    // stack operations
    
    OP_TSX     = 0x19, // transfer SP to X
    OP_TXS     = 0x1a, // transfer X to SP

    OP_PSH_IMM = 0x1b, // push OP on stack
    OP_PSH_IMP = 0x1c,
    OP_PSH_OPX = 0x1d,
    OP_PSH_OPY = 0x1e,
    
    OP_PLL     = 0x1f, // pull stack to A

    OP_POP_IMP = 0x20, // pop stack to OP
    OP_POP_OPX = 0x21,
    OP_POP_OPY = 0x22,

    // bitwise operations

    OP_AND_IMM = 0x23, // A & OP -> A
    OP_AND_OPX = 0x24,
    OP_AND_ABS = 0x25,

    OP_IOR_IMM = 0x26, // A | OP -> A
    OP_IOR_OPX = 0x27,
    OP_IOR_ABS = 0x28,

    OP_XOR_IMM = 0x29, // A ^ OP -> A
    OP_XOR_OPX = 0x2a,
    OP_XOR_ABS = 0x2b,

    OP_SHL_IMM = 0x2c, // A << OP -> A
    OP_SHL_OPX = 0x2d,
    OP_SHL_ABS = 0x2e,

    OP_SHR_IMM = 0x2f, // A >> OP -> A
    OP_SHR_OPX = 0x30,
    OP_SHR_ABS = 0x31,

    OP_NOT_IMP = 0x32, // bitwise negate OP
    OP_NOT_OPX = 0x33,
    OP_NOT_OPY = 0x34,
    OP_NOT_ABS = 0x35,

    // numerical operations

    OP_ADD_IMM = 0x36, // A + OP -> A
    OP_ADD_OPX = 0x37,
    OP_ADD_ABS = 0x38,

    OP_SUB_IMM = 0x39, // A - OP -> A
    OP_SUB_OPX = 0x3a,
    OP_SUB_ABS = 0x3b,

    OP_MUL_IMM = 0x3c, // A * OP -> A
    OP_MUL_OPX = 0x3d,
    OP_MUL_ABS = 0x3e,
    
    OP_DIV_IMM = 0x3f, // A / OP -> A
    OP_DIV_OPX = 0x40,
    OP_DIV_ABS = 0x41,

    OP_INC_IMP = 0x42, // increment OP
    OP_INC_OPX = 0x43,
    OP_INC_OPY = 0x44,
    OP_INC_ABS = 0x45,

    OP_DEC_IMP = 0x46, // decrement OP
    OP_DEC_OPX = 0x47,
    OP_DEC_OPY = 0x48,
    OP_DEC_ABS = 0x49,

    OP_CMP_IMM = 0x4a, // compare A and OP
    OP_CMP_OPX = 0x4b,
    OP_CMP_OPY = 0x4c,
    OP_CMP_ABS = 0x4d,

    // control flow operations

    OP_JMP     = 0x4e, // jump to OP
    OP_JZS     = 0x4f, // jump to OP if zero flag set
    OP_JZN     = 0x50, // jump to OP if zero flag not set
    OP_JCS     = 0x51, // jump to OP if carry flag set
    OP_JCN     = 0x52, // jump to OP if carry flag not set
    OP_JES     = 0x53, // jump to OP if equal flag set
    OP_JEN     = 0x54, // jump to OP if equal flag not set
    OP_JLS     = 0x55, // jump to OP if less than flag set
    OP_JLN     = 0x56, // jump to OP if less than flag not set
    OP_JGS     = 0x57, // jump to OP if greater flag set
    OP_JGN     = 0x58, // jump to OP if greater flag not set
    
    OP_CLL     = 0x59, // call a subroutine at OP
    OP_RET     = 0x5a, // return from a subroutine
    OP_RTI     = 0x5b, // return from an interrupt
    OP_HLT     = 0x5c, // halt execution

} opcode;

char* opcode_to_string(opcode opcode);

#undef Op
#undef IMM
#undef ABS
#undef NOR
