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

static void log_label_node(word a, label_node* n) {
    if (n->is_ident) Log("$%04x: (%s)", a, n->as.ident);
    else Log("$%04x: ($%04x)", a, n->as.literal);
}

static const char* string_of_ins(instruction ins) {
    switch (ins) {
        case INS_LDA: return "lda";
        case INS_LDX: return "ldx";
        case INS_LDY: return "ldy";
        case INS_STA: return "sta";
        case INS_STX: return "stx";
        case INS_STY: return "sty";
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
        case INS_ADD: return "add";
        case INS_SUB: return "sub";
        case INS_MUL: return "mul";
        case INS_DIV: return "div";
        case INS_INA: return "ina";
        case INS_DEA: return "dea";
        case INS_INC: return "inc";
        case INS_DEC: return "dec";
        case INS_JMP: return "jmp";
        case INS_JIZ: return "jiz";
        case INS_JNZ: return "jnz";
        case INS_JIC: return "jic";
        case INS_JNC: return "jnc";
        case INS_CLL: return "cll";
        case INS_RET: return "ret";
        case INS_HLT: return "hlt";
    }
} 

static void log_instr_node(word a, instr_node* n) {
    if (n->has_arg) {
        if (n->arg_is_ident)
            Log("$%04x: %s %s%s", a,
                string_of_ins(n->instr),
                n->arg_is_imm ? "#" : "",
                n->arg_as.ident
            );
        else if (n->arg_is_imm)
            Log("$%04x: %s #$%02x", a,
                string_of_ins(n->instr),
                n->arg_as.literal
            );
        else
            Log("$%04x: %s $%04x", a,
                string_of_ins(n->instr),
                n->arg_as.literal
            );
    } else
        Log("$%04x: %s", a, string_of_ins(n->instr));
}

void log_node(node* n) {
    switch (n->type) {
        case NODE_RAW_DATA:
            log_raw_data_node(n->address, &n->as.raw_data);
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