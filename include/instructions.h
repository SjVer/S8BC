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
    IMM: immediate (literal operands)
    ABS: abssolute (address operands)
*/

#define Op(mode, kind, nibble) \
    (mode << 7 | (kind & 0b111) << 4 | nibble & 0xff)
#define ANY 0
#define IMM 0
#define ABS 1

typedef enum opcode {
    OP_NOP     = 0x00, // no operation

    // load/store operations
    OP_LDA_IMM = Op(IMM, 01, 0x1), // load byte into A
    OP_LDA_ABS = Op(ABS, 01, 0x1),
    OP_LDX_IMM = Op(IMM, 01, 0x2), // load byte into X
    OP_LDX_ABS = Op(ABS, 01, 0x2),
    OP_LDY_IMM = Op(IMM, 01, 0x3), // load byte into Y
    OP_LDY_ABS = Op(ABS, 01, 0x4),
    OP_STA     = Op(ANY, 01, 0x4), // store A in memory
    OP_STX     = Op(ANY, 01, 0x5), // store X in memory
    OP_STY     = Op(ANY, 01, 0x6), // store Y in memory

    // register operations
    OP_TAX     = Op(ANY, 02, 0x1), // transfer A to X
    OP_TAY     = Op(ANY, 02, 0x2), // transfer A to Y
    OP_TXA     = Op(ANY, 02, 0x3), // transfer X to A
    OP_TYA     = Op(ANY, 02, 0x4), // transfer Y to A
    OP_SAX     = Op(ANY, 02, 0x5), // swap A and X
    OP_SAY     = Op(ANY, 02, 0x6), // swap A and Y
    OP_SXY     = Op(ANY, 02, 0x7), // swap X and Y

    // stack operations
    OP_TSX     = Op(ANY, 03, 0x1), // transfer SP to X
    OP_TXS     = Op(ANY, 03, 0x2), // transfer X to SP
    OP_PSH     = Op(ANY, 03, 0x3), // push A on stack
    OP_PLL	   = Op(ANY, 03, 0x4), // pull stack to A
    OP_POP     = Op(ANY, 03, 0x5), // pop stack to A

    // bitwise operations
    OP_AND_IMM = Op(IMM, 04, 0x1), // A & a byte -> A
    OP_AND_ABS = Op(ABS, 04, 0x1),
    OP_IOR_IMM = Op(IMM, 04, 0x2), // A | a byte -> A
    OP_IOR_ABS = Op(ABS, 04, 0x2),
    OP_XOR_IMM = Op(IMM, 04, 0x3), // A ^ a byte -> A
    OP_XOR_ABS = Op(ABS, 04, 0x3),
    OP_SHL_IMM = Op(IMM, 04, 0x4), // A << a byte -> A
    OP_SHL_ABS = Op(ABS, 04, 0x4),
    OP_SHR_IMM = Op(IMM, 04, 0x5), // A >> a byte -> A
    OP_SHR_ABS = Op(ABS, 04, 0x5),

    // numerical operations
    OP_ADD_IMM = Op(IMM, 05, 0x1), // A + a byte -> A
    OP_ADD_ABS = Op(ABS, 05, 0x1),
    OP_SUB_IMM = Op(IMM, 05, 0x2), // A - a byte -> A
    OP_SUB_ABS = Op(ABS, 05, 0x2),
    OP_MUL_IMM = Op(IMM, 05, 0x3), // A * a byte -> A
    OP_MUL_ABS = Op(ABS, 05, 0x3),
    OP_DIV_IMM = Op(IMM, 05, 0x4), // A / a byte -> A
    OP_DIV_ABS = Op(ABS, 05, 0x4),
    OP_INA     = Op(ANY, 05, 0x5), // increment A
    OP_DEA     = Op(ANY, 05, 0x6), // decrement A
    OP_INC     = Op(ANY, 05, 0x7), // increment byte in memory
    OP_DEC     = Op(ANY, 05, 0x8), // decrement byte in memory

    // control flow operations
    OP_JMP     = Op(ANY, 06, 0x1), // jump to an address
    OP_JIZ     = Op(ANY, 06, 0x2), // jump if zero flag set
    OP_JNZ     = Op(ANY, 06, 0x3), // jump if zero flag not set
    OP_JIC     = Op(ANY, 06, 0x4), // jump if carry flag set
    OP_JNC     = Op(ANY, 06, 0x5), // jump if carry flag not set
    OP_CLL     = Op(ANY, 06, 0x6), // call a subroutine
    OP_RET     = Op(ANY, 06, 0x7), // return from a subroutine
    OP_HLT     = Op(ANY, 06, 0x8), // halt execution

} opcode;

char* opcode_to_string(opcode opcode);

#undef Op
#undef IMM
#undef ABS
#undef ANY