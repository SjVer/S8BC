#include <string.h>
#include "as/common.h"
#include "as/parser.h"

token prev_token, curr_token;

void init_parser() {}

static void error_at(token* token, const char* message) {
    if (token->type == TOK_EOF)
        Log_err("%s at end", message);
    else
        Log_err("%s at line %d", message, token->line);

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

static long parse_literal(bool immediate) {
    if (immediate)
        consume(TOK_IMMEDIATE_LITERAL,
            "an immediate literal value");
    else consume(TOK_LITERAL, "a literal value");

    // safe thanks to the lexer
    return strtoul(prev_token.start + immediate, NULL, 10);
}

static node* parse_directive() {
    switch (prev_token.as.dir) {
        case DIR_RESET: {
            error_at(&prev_token, "not yet implemented");
        }

        case DIR_BYTE: {
            long l = parse_literal(false);
            if (l > 0xff) error_at(&prev_token, "byte literal large");

            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = false;
            n->as.raw_data.is_ident = false;
            n->as.raw_data.length = 1;
            n->as.raw_data.as.byte = l;
            return n;
        }
        case DIR_WORD: {
            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = false;
            n->as.raw_data.length = 2;

            if (match(TOK_IDENTIFIER)) {
                char* str = malloc(prev_token.length + 1);
                strncpy(str, prev_token.start, prev_token.length);
                str[prev_token.length] = 0;
                
                n->as.raw_data.is_ident = true;
                n->as.raw_data.as.ident = str;
            } else {
                long l = parse_literal(false);
                if (l > 0xffff) error_at(&prev_token, "word literal too large");

                n->as.raw_data.is_ident = false;
                n->as.raw_data.as.word = l;
            }
            return n;
        }
        case DIR_STRING: {
            consume(TOK_STRING, "a string");
            char* str = malloc(prev_token.length - 1);
            strncpy(str, prev_token.start + 1, prev_token.length - 2);
            str[prev_token.length - 2] = 0;

            node* n = malloc(sizeof(node));
            n->type = NODE_RAW_DATA;
            n->as.raw_data.is_array = true;
            n->as.raw_data.is_ident = false;
            n->as.raw_data.length = prev_token.length - 2;
            n->as.raw_data.as.array = (byte*)str;
            return n;
        }
    }
}

static node* parse_abs_operand(node* n) {
    if (check(TOK_LITERAL)) {
        n->as.instr.arg_is_imm = false;
        n->as.instr.arg_is_ident = false;
        n->as.instr.arg_as.literal = parse_literal(false);
    }
    else if (match(TOK_IDENTIFIER)) {
        n->as.instr.arg_is_imm = false;
        n->as.instr.arg_is_ident = true;
        char* str = malloc(prev_token.length + 1);
        strncpy(str, prev_token.start, prev_token.length);
        str[prev_token.length] = 0;
        n->as.instr.arg_as.ident = str;
    }
    else {
        error_at(&curr_token, "expected an operand");
        return NULL;
    }
    return n;	
}

static node* parse_any_operand(node* n) {
    if (check(TOK_IMMEDIATE_LITERAL)) {
        n->as.instr.arg_is_imm = true;
        n->as.instr.arg_is_ident = false;
        n->as.instr.arg_as.literal = parse_literal(true);
        return n;
    }
    // handles the other two cases
    return parse_abs_operand(n);
}

static node* parse_instruction() {
    node* n = malloc(sizeof(node));
    n->type = NODE_INSTRUCTION;
    n->as.instr.instr = prev_token.as.ins;

    switch (prev_token.as.ins) {
        // with immediate or absolute operand
        case INS_LDA:
        case INS_LDX:
        case INS_LDY:
        case INS_AND:
        case INS_IOR:
        case INS_XOR:
        case INS_SHL:
        case INS_SHR:
        case INS_ADD:
        case INS_SUB:
        case INS_MUL:
        case INS_DIV:
            n->as.instr.has_arg = true;
            return parse_any_operand(n);

        // with absolute operand
        case INS_STA:
        case INS_STX:
        case INS_STY:
        case INS_INC:
        case INS_DEC:
        case INS_JMP:
        case INS_JIZ:
        case INS_JNZ:
        case INS_JIC:
        case INS_JNC:
        case INS_CLL: 
            n->as.instr.has_arg = true;
            return parse_abs_operand(n);
        
        // without operand
        case INS_TAX:
        case INS_TAY:
        case INS_TXA:
        case INS_TYA:
        case INS_SAX:
        case INS_SAY:
        case INS_SXY:
        case INS_TSX:
        case INS_TXS:
        case INS_PSH:
        case INS_PLL:
        case INS_POP:
        case INS_INA:
        case INS_DEA:
        case INS_RET:
        case INS_HLT:
            n->as.instr.has_arg = false;
            n->as.instr.arg_is_imm = false;
            n->as.instr.arg_is_ident = false;
            return n;
    }
}

static node* parse_label() {
    node* n = malloc(sizeof(node));
    n->type = NODE_LABEL;

    if (prev_token.type == TOK_IDENTIFIER) {
        n->as.label.is_ident = true;
        char* str = malloc(prev_token.length + 1);
        strncpy(str, prev_token.start, prev_token.length);
        str[prev_token.length] = 0;
        n->as.label.as.ident = str;
    } else {
        n->as.label.is_ident = false;
        long l = strtoul(prev_token.start, NULL, 10);
        n->as.label.as.literal = l;
    }

    consume(TOK_COLON, "a ':'");
    return n;
}

static node* parse_node() {
    if (match(TOK_DIRECTIVE))
        return parse_directive();

    else if (match(TOK_INSTRUCTION))
        return parse_instruction();

    else if (match(TOK_IDENTIFIER) || match(TOK_LITERAL))
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

        if (prev_node) prev_node->next = next_node;
        else first_node = next_node;
        prev_node = next_node;
    }

    return first_node;
}