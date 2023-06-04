#include "instructions.h"

char* opcode_to_string(opcode opcode) {
    switch (opcode) {
        case OP_NOP:     return "OP_NOP";
        case OP_LDA_IMM: return "OP_LDA_IMM";
        case OP_LDA_ABS: return "OP_LDA_ABS";
        case OP_LDX_IMM: return "OP_LDX_IMM";
        case OP_LDX_ABS: return "OP_LDX_ABS";
        case OP_LDY_IMM: return "OP_LDY_IMM";
        case OP_LDY_ABS: return "OP_LDY_ABS";
        case OP_STA:     return "OP_STA";
        case OP_STX:     return "OP_STX";
        case OP_STY:     return "OP_STY";
        case OP_TAX:     return "OP_TAX";
        case OP_TAY:     return "OP_TAY";
        case OP_TXA:     return "OP_TXA";
        case OP_TYA:     return "OP_TYA";
        case OP_SAX:     return "OP_SAX";
        case OP_SAY:     return "OP_SAY";
        case OP_SXY:     return "OP_SXY";
        case OP_TSX:     return "OP_TSX";
        case OP_TXS:     return "OP_TXS";
        case OP_PSH:     return "OP_PSH";
        case OP_PLL:     return "OP_PLL";
        case OP_POP:     return "OP_POP";
        case OP_AND_IMM: return "OP_AND_IMM";
        case OP_AND_ABS: return "OP_AND_ABS";
        case OP_IOR_IMM: return "OP_IOR_IMM";
        case OP_IOR_ABS: return "OP_IOR_ABS";
        case OP_XOR_IMM: return "OP_XOR_IMM";
        case OP_XOR_ABS: return "OP_XOR_ABS";
        case OP_SHL_IMM: return "OP_SHL_IMM";
        case OP_SHL_ABS: return "OP_SHL_ABS";
        case OP_SHR_IMM: return "OP_SHR_IMM";
        case OP_SHR_ABS: return "OP_SHR_ABS";
        case OP_ADD_IMM: return "OP_ADD_IMM";
        case OP_ADD_ABS: return "OP_ADD_ABS";
        case OP_SUB_IMM: return "OP_SUB_IMM";
        case OP_SUB_ABS: return "OP_SUB_ABS";
        case OP_MUL_IMM: return "OP_MUL_IMM";
        case OP_MUL_ABS: return "OP_MUL_ABS";
        case OP_DIV_IMM: return "OP_DIV_IMM";
        case OP_DIV_ABS: return "OP_DIV_ABS";
        case OP_INA:     return "OP_INA";
        case OP_DEA:     return "OP_DEA";
        case OP_INC:     return "OP_INC";
        case OP_DEC:     return "OP_DEC";
        case OP_JMP:     return "OP_JMP";
        case OP_JIZ:     return "OP_JIZ";
        case OP_JNZ:     return "OP_JNZ";
        case OP_JIC:     return "OP_JIC";
        case OP_JNC:     return "OP_JNC";
        case OP_CLL:     return "OP_CLL";
        case OP_RET:     return "OP_RET";
        case OP_HLT:     return "OP_HLT";
    }
}
