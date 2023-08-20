#pragma once
#define AS_LEXER_H

typedef enum {
    TOK_COLON,
    TOK_EQUAL,
    TOK_DIRECTIVE,
    TOK_INSTRUCTION,
    TOK_REGISTER_X,
    TOK_REGISTER_Y,
    TOK_ABS_LITERAL,
    TOK_IMM_LITERAL,
    TOK_REL_LITERAL,
    TOK_ABS_IDENTIFIER,
    TOK_IMM_IDENTIFIER,
    TOK_STRING,
    TOK_EOF
} token_type;

typedef enum {
    DIR_BYTE,
    DIR_WORD,
    DIR_STRING,
} directive;

typedef enum {
    INS_NOP,
    
    INS_LDA,
    INS_LDX,
    INS_LDY,
    INS_LDI,
    INS_STA,
    INS_STX,
    INS_STY,
    INS_STI,

    INS_TAX,
    INS_TAY,
    INS_TXA,
    INS_TYA,
    INS_SAX,
    INS_SAY,
    INS_SXY,

    INS_TSX,
    INS_TXS,
    INS_PSH,
    INS_PLL,
    INS_POP,

    INS_AND,
    INS_IOR,
    INS_XOR,
    INS_SHL,
    INS_SHR,
    INS_NOT,

    INS_ADD,
    INS_SUB,
    INS_MUL,
    INS_DIV,
    INS_INC,
    INS_DEC,
    INS_CMP,

    INS_JMP,
    INS_JZS,
    INS_JZN,
    INS_JCS,
    INS_JCN,
    INS_JES,
    INS_JEN,
    INS_JLS,
    INS_JLN,
    INS_JGS,
    INS_JGN,
    INS_CLL,
    INS_RET,
    INS_RTI,
    INS_HLT,
} instruction;

typedef union token_as token_as;

typedef struct {
    token_type type;
    const char* start;
    int line, length;
    
    union token_as {
        directive dir;
        instruction ins;
        const char* error;
        long literal;
    } as;
} token;

void init_lexer(const char* source);
token scan_next_token();
