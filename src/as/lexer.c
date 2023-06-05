#include <string.h>
#include "as/common.h"
#include "as/lexer.h"

const char* start;
const char* current;
int line;

void init_lexer(const char* source) {
    start = source;
    current = start;
    line = 1;
}

static bool is_at_end() {
    return *current == '\0';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static char advance() {
    current++;
    return current[-1];
}

static char peek() {
    return *current;
}

static token make_token(token_type type, token_as as) {
    token token;
    token.type = type;
    token.start = start;
    token.line = line;
    token.length = (int)(current - start);
    token.as = as;
    return token;
}

static token error_token(const char *message) {
    return make_token(TOK_ERROR, (token_as){.error = message});
}

static token maybe_directive() {
    while (is_alpha(peek()) || is_digit(peek())) advance();
    int length = current - start;

#define Match(str, dir_)                                              \
    if (length == strlen("." str) && strncmp(start, "." str, length)) \
        return make_token(TOK_DIRECTIVE, (token_as){.dir = dir_});    \

    Match("reset",  DIR_RESET);
    Match("byte",   DIR_BYTE);
    Match("word",   DIR_WORD);
    Match("string", DIR_STRING);

#undef Match

    return error_token("invalid directive");
}

static token instruction_or_identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();

    if (current - start == 3) {
#define Match_instr(str, inst)                                       \
    if (!strncmp(start, str, 3))                                     \
        return make_token(TOK_INSTRUCTION, (token_as){.ins = inst}); 

        Match_instr("lda", INS_LDA);
        Match_instr("ldx", INS_LDX);
        Match_instr("ldy", INS_LDY);
        Match_instr("sta", INS_STA);
        Match_instr("stx", INS_STX);
        Match_instr("sty", INS_STY);

        Match_instr("tax", INS_TAX);
        Match_instr("tay", INS_TAY);
        Match_instr("txa", INS_TXA);
        Match_instr("tya", INS_TYA);
        Match_instr("sax", INS_SAX);
        Match_instr("say", INS_SAY);
        Match_instr("sxy", INS_SXY);

        Match_instr("tsx", INS_TSX);
        Match_instr("txs", INS_TXS);
        Match_instr("psh", INS_PSH);
        Match_instr("pll", INS_PLL);
        Match_instr("pop", INS_POP);

        Match_instr("and", INS_AND);
        Match_instr("ior", INS_IOR);
        Match_instr("xor", INS_XOR);
        Match_instr("shl", INS_SHL);
        Match_instr("shr", INS_SHR);

        Match_instr("add", INS_ADD);
        Match_instr("sub", INS_SUB);
        Match_instr("mul", INS_MUL);
        Match_instr("div", INS_DIV);
        Match_instr("ina", INS_INA);
        Match_instr("dea", INS_DEA);
        Match_instr("inc", INS_INC);
        Match_instr("dec", INS_DEC);

        Match_instr("jmp", INS_JMP);
        Match_instr("jiz", INS_JIZ);
        Match_instr("jnz", INS_JNZ);
        Match_instr("jic", INS_JIC);
        Match_instr("jnc", INS_JNC);
        Match_instr("cll", INS_CLL);
        Match_instr("ret", INS_RET);
        Match_instr("hlt", INS_HLT);
    
#undef Match_instr
    }

    return make_token(TOK_IDENTIFIER, (token_as){0});
}

static token string() {
    while (peek() != '"' && !is_at_end())
    {
        if (peek() == '\n') line++;
        advance();
    }

    if (is_at_end())
        return error_token("unterminated string");

    // The closing quote.
    advance();
    return make_token(TOK_STRING, (token_as){0});
}

static token number(bool is_immediate) {
    while (is_digit(peek())) advance();

    token_type type = is_immediate ? TOK_IMMEDIATE_LITERAL : TOK_LITERAL;
    return make_token(type, (token_as){0});
}

static void skip_whitespaces() {
    for (;;)
    {
        char c = peek();
        switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            line++;
            advance();
            break;
        // comment maybe
        case ';':
            while (peek() != '\n' && !is_at_end())
                advance();
            break;
        default:
            return;
        }
    }
}

token scan_next_token() {
    skip_whitespaces();

    start = current;

    if (is_at_end())
        return make_token(TOK_EOF, (token_as){0});

    char c = advance();

    // directives
    if (c == '.') return maybe_directive();

    // identifiers/instructions
    else if (is_alpha(c)) return instruction_or_identifier();

    // numbers
    else if (is_digit(c)) return number(false);
    else if (c == '#') {
        if (!is_digit(advance()))
            return error_token("expected an immediate value");
        return number(true);
    }

    // strings
    else if (c == '"') return string();

    // colon
    else if (c == ':') return
        make_token(TOK_COLON, (token_as){0});

    return error_token("unexpected character");
}