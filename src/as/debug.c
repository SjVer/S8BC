#include "as/common.h"
#include "as/debug.h"

static void log_raw_data_node(word a, raw_data_node* n) {
    if (n->is_array) // string
        Log("$%04x: .string \"%.*s\"", a, n->length, n->as.array);
    else if (n->length == 1) // byte
        Log("$%04x: .byte $%02x", a, n->as.byte);
    else if (n->length == 2 && n->is_ident) // word
        Log("$%04x: .word %s", a, n->as.ident);
    else if (n->length == 2) // word
        Log("$%04x: .word $%04x", a, n->as.word);
}

static void log_alias_node(alias_node* n) {
    Log("(%s = $%04x)", n->ident, n->address);
}

static void log_label_node(word a, label_node* n) {
    if (n->is_ident) Log("$%04x: (%s)", a, n->as.ident);
    else Log("$%04x: ($%04x)", a, n->as.literal);
}

const char* string_of_ins(instruction ins) {
    switch (ins) {
        case INS_NOP: return "nop";
        case INS_LDA: return "lda";
        case INS_LDX: return "ldx";
        case INS_LDY: return "ldy";
        case INS_STA: return "sta";
        case INS_STX: return "stx";
        case INS_STY: return "sty";
        case INS_STI: return "sti";
        case INS_TAX: return "tax";
        case INS_TAY: return "tay";
        case INS_TXA: return "txa";
        case INS_TYA: return "tya";
        case INS_SAX: return "sax";
        case INS_SAY: return "say";
        case INS_SXY: return "sxy";
        case INS_TSX: return "tsx";
        case INS_TXS: return "txs";
        case INS_PSH: return "psh";
        case INS_PLL: return "pll";
        case INS_POP: return "pop";
        case INS_AND: return "and";
        case INS_IOR: return "ior";
        case INS_XOR: return "xor";
        case INS_SHL: return "shl";
        case INS_SHR: return "shr";
        case INS_NOT: return "not";
        case INS_ADD: return "add";
        case INS_SUB: return "sub";
        case INS_MUL: return "mul";
        case INS_DIV: return "div";
        case INS_INC: return "inc";
        case INS_DEC: return "dec";
        case INS_CMP: return "cmp";
        case INS_JMP: return "jmp";
        case INS_JZS: return "jzs";
        case INS_JZN: return "jzn";
        case INS_JCS: return "jcs";
        case INS_JCN: return "jcn";
        case INS_JES: return "jes";
        case INS_JEN: return "jen";
        case INS_JLS: return "jls";
        case INS_JLN: return "jln";
        case INS_JGS: return "jgs";
        case INS_JGN: return "jgn";
        case INS_CLL: return "cll";
        case INS_RET: return "ret";
        case INS_RTI: return "rti";
        case INS_HLT: return "hlt";
        
        default: return "???";
    }
} 

static void log_instr_node(word a, instr_node* n) {
    if (n->arg_type) {
        if (n->arg_type == TOK_IMM_IDENTIFIER)
            Log("$%04x: %s #%s", a,
                string_of_ins(n->instr),
                n->as.ident);
        else if (n->arg_type == TOK_IMM_LITERAL)
            Log("$%04x: %s #$%02x", a,
                string_of_ins(n->instr),
                n->as.imm_literal);

        else if (n->arg_type == TOK_REGISTER_X)
            Log("$%04x: %s x", a, string_of_ins(n->instr));
        else if (n->arg_type == TOK_REGISTER_Y)
            Log("$%04x: %s y", a, string_of_ins(n->instr));
        else if (n->arg_type == TOK_REL_LITERAL)
            Log("$%04x: %s @%d", a, 
                string_of_ins(n->instr),
                n->as.rel_literal);

        else if (n->arg_type == TOK_ABS_IDENTIFIER)
            Log("$%04x: %s %s", a,
                string_of_ins(n->instr),
                n->as.ident);
        else if (n->arg_type == TOK_ABS_LITERAL)
            Log("$%04x: %s $%04x", a,
                string_of_ins(n->instr),
                n->as.abs_literal);
                
        else {
            Log_err("cannot log invalid operand (%d)\n", n->arg_type);
            Abort(STATUS_INTERNAL_ERROR);
        }
    } else
        Log("$%04x: %s", a, string_of_ins(n->instr));
}

void log_node(node* n) {
    switch (n->type) {
        case NODE_RAW_DATA:
            log_raw_data_node(n->address, &n->as.raw_data);
            break;
        case NODE_ALIAS:
            log_alias_node(&n->as.alias);
            break;
        case NODE_LABEL:
            log_label_node(n->address, &n->as.label);
            break;
        case NODE_INSTRUCTION:
            log_instr_node(n->address, &n->as.instr);
            break;
    }
}

void log_nodes(node* n) {
    putchar('\n');

    node* curr = n;
    while (curr) {
        log_node(curr);
        curr = curr->next;
    }

    putchar('\n');
}
