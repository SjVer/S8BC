#include "instructions.h"

char* opcode_to_string(opcode opcode) {
    switch (opcode) {
        case OP_NOP:     return "nop";
        case OP_LDA_IMM: return "lda imm";
        case OP_LDA_OPX: return "lda opx";
        case OP_LDA_OPY: return "lda opy";
        case OP_LDA_ABS: return "lda abs";
        case OP_LDX_IMM: return "ldx imm";
        case OP_LDX_OPY: return "ldx opy";
        case OP_LDX_ABS: return "ldx abs";
        case OP_LDY_IMM: return "ldy imm";
        case OP_LDY_OPX: return "ldy opx";
        case OP_LDY_ABS: return "ldy abs";
        case OP_LDI_ABS: return "ldi abs";
        case OP_STA_OPX: return "sta opx";
        case OP_STA_OPY: return "sta opy";
        case OP_STA_ABS: return "sta abs";
        case OP_STX_ABS: return "stx abs";
        case OP_STY_ABS: return "sty abs";
        case OP_STI_ABS: return "sti abs";
        case OP_TAX:     return "tax";
        case OP_TAY:     return "tay";
        case OP_TXA:     return "txa";
        case OP_TYA:     return "tya";
        case OP_SAX:     return "sax";
        case OP_SAY:     return "say";
        case OP_SXY:     return "sxy";
        case OP_TSX:     return "tsx";
        case OP_TXS:     return "txs";
        case OP_PSH_IMM: return "psh imm";
        case OP_PSH_IMP: return "psh imp";
        case OP_PSH_OPX: return "psh opx";
        case OP_PSH_OPY: return "psh opy";
        case OP_PLL:     return "pll";
        case OP_POP:     return "pop";
        case OP_AND_IMM: return "and imm";
        case OP_AND_OPX: return "and opx";
        case OP_AND_ABS: return "and abs";
        case OP_IOR_IMM: return "ior imm";
        case OP_IOR_OPX: return "ior opx";
        case OP_IOR_ABS: return "ior abs";
        case OP_XOR_IMM: return "xor imm";
        case OP_XOR_OPX: return "xor opx";
        case OP_XOR_ABS: return "xor abs";
        case OP_SHL_IMM: return "shl imm";
        case OP_SHL_OPX: return "shl opx";
        case OP_SHL_ABS: return "shl abs";
        case OP_SHR_IMM: return "shr imm";
        case OP_SHR_OPX: return "shr opx";
        case OP_SHR_ABS: return "shr abs";
        case OP_ADD_IMM: return "add imm";
        case OP_ADD_OPX: return "add opx";
        case OP_ADD_ABS: return "add abs";
        case OP_SUB_IMM: return "sub imm";
        case OP_SUB_OPX: return "sub opx";
        case OP_SUB_ABS: return "sub abs";
        case OP_MUL_IMM: return "mul imm";
        case OP_MUL_OPX: return "mul opx";
        case OP_MUL_ABS: return "mul abs";
        case OP_DIV_IMM: return "div imm";
        case OP_DIV_OPX: return "div opx";
        case OP_DIV_ABS: return "div abs";
        case OP_INC_IMP: return "inc imp";
        case OP_INC_OPX: return "inc opx";
        case OP_INC_OPY: return "inc opy";
        case OP_INC_ABS: return "inc abs";
        case OP_DEC_IMP: return "dec imp";
        case OP_DEC_OPX: return "dec opx";
        case OP_DEC_OPY: return "dec opy";
        case OP_DEC_ABS: return "dec abs";
        case OP_JMP:     return "jmp";
        case OP_JIZ:     return "jiz";
        case OP_JNZ:     return "jnz";
        case OP_JIC:     return "jic";
        case OP_JNC:     return "jnc";
        case OP_CLL:     return "cll";
        case OP_RET:     return "ret";
        case OP_HLT:     return "hlt";

        default: return "???";
    }
}
