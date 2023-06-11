#include <string.h>
#include "as/common.h"
#include "as/parser.h"

token prev_token, curr_token;

void init_parser() {}

static void error_at(token* t, const char* message) {
    if (t->type == TOK_EOF)
        Log_err("%s at end", message);
    else {
        Log_err("%s at line %d", message, t->line);
        Log_err("(erroneous token: '%.*s')", t->length, t->start);
    }

    Abort(STATUS_PARSE_ERROR);
}

static void advance() {
    prev_token = curr_token;
    curr_token = scan_next_token();
}

static bool check(token_type type) {
    return curr_token.type == type;
}

static bool match(token_type type) {
    if (!check(type))
        return false;

    advance();
    return true;
}

static void consume(token_type type, const char* what) {
    if (!match(type)) {
        char buff[100] = {0};
        snprintf(buff, 100, "expected %s", what);
        error_at(&curr_token, buff);
    }
}

static char* copy_identifier(const char* start, int length) {
    char* str = malloc(length + 1);
    strncpy(str, start, length);
    str[length] = 0;
    return str;
}

static node* parse_directive() {
    switch (prev_token.as.dir) {
        case DIR_BYTE: {
            consume(TOK_IMM_LITERAL, "an immediate literal");

            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = false;
            n->as.raw_data.is_ident = false;
            n->as.raw_data.length = 1;
            n->as.raw_data.as.byte = prev_token.as.literal;
            return n;
        }
        case DIR_WORD: {
            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = false;
            n->as.raw_data.length = 2;

            if (match(TOK_IMM_IDENTIFIER)) {
                n->as.raw_data.is_ident = true;
                n->as.raw_data.as.ident = copy_identifier(
                    prev_token.start + 1, prev_token.length - 1);
            } else {
                n->as.raw_data.is_ident = false;
                n->as.raw_data.as.word = prev_token.as.literal;
            }
            return n;
        }
        case DIR_STRING: {
            consume(TOK_STRING, "a string");

            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = true;
            n->as.raw_data.is_ident = false;
            n->as.raw_data.length = prev_token.length - 2;
            n->as.raw_data.as.array = (byte*)copy_identifier(
                prev_token.start + 1, prev_token.length - 2);
            return n;
        }
    }
}

enum {
    OP_NO_OP = 0,
    OP_IMM = 1 << 0,
    OP_IMP = 1 << 1,
    OP_OPX = 1 << 2,
    OP_OPY = 1 << 3,
    OP_ABS = 1 << 4,
};

static void parse_operand(instr_node* i, unsigned ops) {
    i->arg_type = curr_token.type;

    if (ops & OP_IMM && match(TOK_IMM_LITERAL))
        i->as.imm_literal = prev_token.as.literal;

    else if (ops & OP_IMM && match(TOK_IMM_IDENTIFIER))
        i->as.ident = copy_identifier(
            prev_token.start + 1,
            prev_token.length - 1);

    else if (ops & OP_OPX && match(TOK_REGISTER_X)) return;
    else if (ops & OP_OPY && match(TOK_REGISTER_Y)) return;

    else if (ops & OP_ABS && match(TOK_ABS_LITERAL))
        i->as.imm_literal = prev_token.as.literal;

    else if (ops & OP_ABS && match(TOK_ABS_IDENTIFIER))
        i->as.ident = copy_identifier(
            prev_token.start,
            prev_token.length);
            
    else if (ops & OP_IMP || ops == OP_NO_OP)
        i->arg_type = 0;
    
    else error_at(&curr_token, "expected an operand");
}

static node* parse_instruction() {
    node* n = malloc(sizeof(node));
    n->type = NODE_INSTRUCTION;
    n->as.instr.instr = prev_token.as.ins;

#define Op(types) parse_operand(&n->as.instr, types); break

    switch (prev_token.as.ins) {
        
        // load/store operations
        case INS_LDA: Op(OP_IMM | OP_OPX | OP_OPY | OP_ABS);
        case INS_LDX: Op(OP_IMM | OP_OPY | OP_ABS);
        case INS_LDY: Op(OP_IMM | OP_OPX | OP_ABS);
        case INS_LDI: Op(OP_ABS);
        case INS_STA: Op(OP_OPX | OP_OPY | OP_ABS);
        case INS_STX:
        case INS_STY:
        case INS_STI: Op(OP_ABS);

        // register operations
        case INS_TAX:
        case INS_TAY:
        case INS_TXA:
        case INS_TYA:
        case INS_SAX:
        case INS_SAY:
        case INS_SXY: Op(OP_NO_OP);

        // stack operations
        case INS_TSX:
        case INS_TXS: Op(OP_NO_OP);
        case INS_PSH: Op(OP_IMM | OP_IMP | OP_OPX | OP_OPY);
        case INS_PLL:
        case INS_POP: Op(OP_NO_OP);

        // bitwise operations
        case INS_AND:
        case INS_IOR:
        case INS_XOR:
        case INS_SHL:
        case INS_SHR: Op(OP_IMM | OP_OPX | OP_ABS);

        // numerical operations
        case INS_ADD:
        case INS_SUB:
        case INS_MUL:
        case INS_DIV: Op(OP_IMM | OP_OPX | OP_ABS);
        case INS_INC:
        case INS_DEC: Op(OP_IMP | OP_OPX | OP_OPY | OP_ABS);

        // control flow operations
        case INS_JMP:
        case INS_JIZ:
        case INS_JNZ:
        case INS_JIC:
        case INS_JNC:
        case INS_CLL: Op(OP_ABS);
        case INS_RET:
        case INS_HLT: Op(OP_NO_OP);
    }

#undef Op

    return n;
}

static node* parse_label() {
    node* n = malloc(sizeof(node));
    n->type = NODE_LABEL;

    if (prev_token.type == TOK_ABS_IDENTIFIER) {
        n->as.label.is_ident = true;
        n->as.label.as.ident = copy_identifier(
            prev_token.start, prev_token.length);
    } else {
        n->as.label.is_ident = false;
        n->as.label.as.literal = prev_token.as.literal;
    }

    consume(TOK_COLON, "a ':'");
    return n;
}

static node* parse_node() {
    if (match(TOK_DIRECTIVE))
        return parse_directive();

    else if (match(TOK_INSTRUCTION))
        return parse_instruction();

    else if (match(TOK_ABS_IDENTIFIER) || match(TOK_ABS_LITERAL))
        return parse_label();

    error_at(&curr_token, "unexpected token");
    return NULL;
}

node* parse() {
    // sets up curr_token
    advance();
    
    node* first_node = NULL;
    node* prev_node = NULL;
    
    while (!check(TOK_EOF)) {
        node* next_node = parse_node();

        // invalid node
        if (!next_node) continue;

        if (prev_node) prev_node->next = next_node;
        else first_node = next_node;
        prev_node = next_node;
    }

    return first_node;
}