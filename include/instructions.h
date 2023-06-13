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
    OP_NOP,     // no operation

    // load/store operations

    OP_LDA_IMM, // load OP into A
    OP_LDA_OPX,
    OP_LDA_OPY,
    OP_LDA_ABS,

    OP_LDX_IMM, // load OP into X
    OP_LDX_OPY,
    OP_LDX_ABS,

    OP_LDY_IMM, // load OP into Y
    OP_LDY_OPX,
    OP_LDY_ABS,

    OP_LDI_ABS, // load byte at address at OP into A

    OP_STA_OPX, // store A at OP
    OP_STA_OPY,
    OP_STA_ABS,

    OP_STX_ABS, // store X at OP
    OP_STY_ABS, // store Y at OP
    OP_STI_ABS, // store A at address at OP

    // register operations

    OP_TAX,     // transfer A to X
    OP_TAY,     // transfer A to Y
    OP_TXA,     // transfer X to A
    OP_TYA,     // transfer Y to A
    OP_SAX,     // swap A and X
    OP_SAY,     // swap A and Y
    OP_SXY,     // swap X and Y

    // stack operations
    
    OP_TSX,     // transfer SP to X
    OP_TXS,     // transfer X to SP

    OP_PSH_IMM, // push OP on stack
    OP_PSH_IMP,
    OP_PSH_OPX,
    OP_PSH_OPY,
    
    OP_PLL,	    // pull stack to A

    OP_POP_IMP, // pop stack to OP
    OP_POP_OPX,
    OP_POP_OPY,

    // bitwise operations

    OP_AND_IMM, // A & OP -> A
    OP_AND_OPX,
    OP_AND_ABS,

    OP_IOR_IMM, // A | OP -> A
    OP_IOR_OPX,
    OP_IOR_ABS,

    OP_XOR_IMM, // A ^ OP -> A
    OP_XOR_OPX,
    OP_XOR_ABS,

    OP_SHL_IMM, // A << OP -> A
    OP_SHL_OPX,
    OP_SHL_ABS,

    OP_SHR_IMM, // A >> OP -> A
    OP_SHR_OPX,
    OP_SHR_ABS,

    OP_NOT_IMP, // bitwise negate OP
    OP_NOT_OPX,
    OP_NOT_OPY,
    OP_NOT_ABS,

    // numerical operations

    OP_ADD_IMM, // A + OP -> A
    OP_ADD_OPX,
    OP_ADD_ABS,

    OP_SUB_IMM, // A - OP -> A
    OP_SUB_OPX,
    OP_SUB_ABS,

    OP_MUL_IMM, // A * OP -> A
    OP_MUL_OPX,
    OP_MUL_ABS,
    
    OP_DIV_IMM, // A / OP -> A
    OP_DIV_OPX,
    OP_DIV_ABS,

    OP_INC_IMP, // increment OP
    OP_INC_OPX,
    OP_INC_OPY,
    OP_INC_ABS,

    OP_DEC_IMP, // decrement OP
    OP_DEC_OPX,
    OP_DEC_OPY,
    OP_DEC_ABS,

    OP_CMP_IMM, // compare A and OP
    OP_CMP_OPX,
    OP_CMP_OPY,
    OP_CMP_ABS,

    // control flow operations

    OP_JMP,     // jump to OP
    OP_JZS,     // jump to OP if zero flag set
    OP_JZN,     // jump to OP if zero flag not set
    OP_JCS,     // jump to OP if carry flag set
    OP_JCN,     // jump to OP if carry flag not set
    OP_JES,     // jump to OP if equal flag set
    OP_JEN,     // jump to OP if equal flag not set
    OP_JLS,     // jump to OP if less than flag set
    OP_JLN,     // jump to OP if less than flag not set
    OP_JGS,     // jump to OP if greater flag set
    OP_JGN,     // jump to OP if greater flag not set
    
    OP_CLL,     // call a subroutine at OP
    OP_RET,     // return from a subroutine
    OP_RTI,     // return from an interrupt
    OP_HLT,     // halt execution

} opcode;

char* opcode_to_string(opcode opcode);

#undef Op
#undef IMM
#undef ABS
#undef NOR
