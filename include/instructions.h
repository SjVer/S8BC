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
    STK: stack (n'th item in stack)
    ABS: absolute (byte at literal address)
*/

typedef enum {
    OP_NOP = 0x00, // no operation

    // load/store operations

    OP_LDA_IMM = 0x01, // load OP into A
    OP_LDA_OPX = 0x02,
    OP_LDA_OPY = 0x03,
    OP_LDA_STK = 0x04,
    OP_LDA_ABS = 0x05,

    OP_LDX_IMM = 0x06, // load OP into X
    OP_LDX_OPY = 0x07,
    OP_LDX_STK = 0x08,
    OP_LDX_ABS = 0x09,

    OP_LDY_IMM = 0x0a, // load OP into Y
    OP_LDY_OPX = 0x0b,
    OP_LDY_STK = 0x0c,
    OP_LDY_ABS = 0x0d,

    OP_LDI_ABS = 0x0e, // load byte at address at OP into A

    OP_STA_OPX = 0x0f, // store A at OP
    OP_STA_OPY = 0x10,
    OP_STA_STK = 0x11,
    OP_STA_ABS = 0x12,

    OP_STX_ABS = 0x13, // store X at OP
    OP_STY_ABS = 0x14, // store Y at OP
    OP_STI_ABS = 0x15, // store A at address at OP

    // register operations

    OP_TAX = 0x16, // transfer A to X
    OP_TAY = 0x17, // transfer A to Y
    OP_TXA = 0x18, // transfer X to A
    OP_TYA = 0x19, // transfer Y to A
    OP_SAX = 0x1a, // swap A and X
    OP_SAY = 0x1b, // swap A and Y
    OP_SXY = 0x1c, // swap X and Y

    // stack operations
    
    OP_TSX = 0x1d, // transfer SP to X
    OP_TXS = 0x1e, // transfer X to SP

    OP_PSH_IMM = 0x1f, // push OP on stack
    OP_PSH_IMP = 0x20,
    OP_PSH_OPX = 0x21,
    OP_PSH_OPY = 0x22,
    
    OP_PLL = 0x23, // pull stack to A

    OP_POP_IMP = 0x24, // pop stack to OP
    OP_POP_OPX = 0x25,
    OP_POP_OPY = 0x26,

    // bitwise operations

    OP_AND_IMM = 0x27, // A & OP -> A
    OP_AND_OPX = 0x28,
    OP_AND_ABS = 0x29,

    OP_IOR_IMM = 0x2a, // A | OP -> A
    OP_IOR_OPX = 0x2b,
    OP_IOR_ABS = 0x2c,

    OP_XOR_IMM = 0x2d, // A ^ OP -> A
    OP_XOR_OPX = 0x2e,
    OP_XOR_ABS = 0x2f,

    OP_SHL_IMM = 0x30, // A << OP -> A
    OP_SHL_OPX = 0x31,
    OP_SHL_ABS = 0x32,

    OP_SHR_IMM = 0x33, // A >> OP -> A
    OP_SHR_OPX = 0x34,
    OP_SHR_ABS = 0x35,

    OP_NOT_IMP = 0x36, // bitwise negate OP
    OP_NOT_OPX = 0x37,
    OP_NOT_OPY = 0x38,
    OP_NOT_ABS = 0x39,

    // numerical operations

    OP_ADD_IMM = 0x3a, // A + OP -> A
    OP_ADD_OPX = 0x3b,
    OP_ADD_ABS = 0x3c,

    OP_SUB_IMM = 0x3d, // A - OP -> A
    OP_SUB_OPX = 0x3e,
    OP_SUB_ABS = 0x3f,

    OP_MUL_IMM = 0x40, // A * OP -> A
    OP_MUL_OPX = 0x41,
    OP_MUL_ABS = 0x42,
    
    OP_DIV_IMM = 0x43, // A / OP -> A
    OP_DIV_OPX = 0x44,
    OP_DIV_ABS = 0x45,

    OP_INC_IMP = 0x46, // increment OP
    OP_INC_OPX = 0x47,
    OP_INC_OPY = 0x48,
    OP_INC_ABS = 0x49,

    OP_DEC_IMP = 0x4a, // decrement OP
    OP_DEC_OPX = 0x4b,
    OP_DEC_OPY = 0x4c,
    OP_DEC_ABS = 0x4d,

    OP_CMP_IMM = 0x4e, // compare A and OP
    OP_CMP_OPX = 0x4f,
    OP_CMP_OPY = 0x50,
    OP_CMP_ABS = 0x51,

    // control flow operations

    OP_JMP = 0x52, // jump to OP
    OP_JZS = 0x53, // jump to OP if zero flag set
    OP_JZN = 0x54, // jump to OP if zero flag not set
    OP_JCS = 0x55, // jump to OP if carry flag set
    OP_JCN = 0x56, // jump to OP if carry flag not set
    OP_JES = 0x57, // jump to OP if equal flag set
    OP_JEN = 0x58, // jump to OP if equal flag not set
    OP_JLS = 0x59, // jump to OP if less than flag set
    OP_JLN = 0x5a, // jump to OP if less than flag not set
    OP_JGS = 0x5b, // jump to OP if greater flag set
    OP_JGN = 0x5c, // jump to OP if greater flag not set
    
    OP_CLL = 0x5d, // call a subroutine at OP
    OP_RET = 0x5e, // return from a subroutine
    OP_RTI = 0x5f, // return from an interrupt
    OP_HLT = 0x60, // halt execution

} opcode;

char* opcode_to_string(opcode opcode);

#undef Op
#undef IMM
#undef ABS
#undef NOR
