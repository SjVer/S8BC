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

  modes:
    IMM: immediate (literal operators)
    ABS: abssolute (address operators)
*/

#define OP(mode, nibble, args) \
    (mode << 7 | nibble << 3 | args & 0b111)
#define IMM 0
#define ABS 1

typedef enum opcode {
    OP_NOP     = 0x00, // no operation

    // load/store operations
    OP_LDA_IMM = OP(IMM, 0xA, 1), // load byte into A
    OP_LDA_ABS = OP(ABS, 0xA, 1),
    OP_LDX_IMM = OP(IMM, 0xA, 2), // load byte into X
    OP_LDX_ABS = OP(ABS, 0xA, 2),
    OP_LDY_IMM = OP(IMM, 0xA, 3), // load byte into Y
    OP_LDY_ABS = OP(ABS, 0xA, 4),
    OP_STA     = OP(IMM, 0xA, 4), // store A in memory
    OP_STX     = OP(IMM, 0xA, 5), // store X in memory
    OP_STY     = OP(IMM, 0xA, 6), // store Y in memory

    // register operations
    OP_TAX     = OP(IMM, 0xB, 1), // transfer A to X
    OP_TAY     = OP(IMM, 0xB, 2), // transfer A to Y
    OP_TXA     = OP(IMM, 0xB, 3), // transfer X to A
    OP_TYA     = OP(IMM, 0xB, 4), // transfer Y to A
    OP_SAX     = OP(IMM, 0xB, 5), // swap A and X
    OP_SAY     = OP(IMM, 0xB, 6), // swap A and Y
    OP_SXY     = OP(IMM, 0xB, 7), // swap X and Y

    // stack operations
    OP_TSX     = OP(IMM, 0xC, 1), // transfer SP to X
    OP_TXS     = OP(IMM, 0xC, 2), // transfer X to SP
    OP_PSH     = OP(IMM, 0xC, 3), // push A on stack
    OP_PLL	   = OP(IMM, 0xC, 4), // pull stack to A
    OP_POP     = OP(IMM, 0xC, 5), // pop stack to A

    // bitwise operations
    OP_AND_IMM = OP(IMM, 0xD, 1), // A & a byte -> A
    OP_AND_ABS = OP(ABS, 0xD, 1),
    OP_IOR_IMM = OP(IMM, 0xD, 2), // A | a byte -> A
    OP_IOR_ABS = OP(ABS, 0xD, 2),
    OP_XOR_IMM = OP(IMM, 0xD, 3), // A ^ a byte -> A
    OP_XOR_ABS = OP(ABS, 0xD, 3),
    OP_SHL_IMM = OP(IMM, 0xD, 4), // A << a byte -> A
    OP_SHL_ABS = OP(ABS, 0xD, 4),
    OP_SHR_IMM = OP(IMM, 0xD, 5), // A >> a byte -> A
    OP_SHR_ABS = OP(ABS, 0xD, 5),

    // numerical operations
    OP_ADD_IMM = OP(IMM, 0xE, 1), // A + a byte -> A
    OP_ADD_ABS = OP(ABS, 0xE, 1),
    OP_SUB_IMM = OP(IMM, 0xE, 2), // A - a byte -> A
    OP_SUB_ABS = OP(ABS, 0xE, 2),
    OP_MUL_IMM = OP(IMM, 0xE, 3), // A * a byte -> A
    OP_MUL_ABS = OP(ABS, 0xE, 3),
    OP_DIV_IMM = OP(IMM, 0xE, 4), // A / a byte -> A
    OP_DIV_ABS = OP(ABS, 0xE, 4),
    OP_INA     = OP(IMM, 0xE, 5), // increment A
    OP_DEA     = OP(IMM, 0xE, 6), // decrement A
    OP_INC     = OP(IMM, 0xE, 7), // increment byte in memory
    OP_DEC     = OP(IMM, 0xE, 8), // decrement byte in memory

    // control flow operations
    OP_JMP     = OP(IMM, 0xF, 1), // jump to an address
    OP_JIZ     = OP(IMM, 0xF, 2), // jump if zero flag set
    OP_JNZ     = OP(IMM, 0xF, 3), // jump if zero flag not set
    OP_JIC     = OP(IMM, 0xF, 4), // jump if carry flag set
    OP_JNC     = OP(IMM, 0xF, 5), // jump if carry flag not set
    OP_CLL     = OP(IMM, 0xF, 6), // call a subroutine
    OP_RET     = OP(IMM, 0xF, 7), // return from a subroutine
    OP_HLT     = OP(IMM, 0xF, 8), // halt execution

} opcode;

char* opcode_to_string(opcode opcode);

#undef OP
#undef IMM
#undef ABS