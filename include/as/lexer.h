#pragma once
#define AS_LEXER_H

typedef enum {
    TOK_COLON,
    TOK_DIRECTIVE,
    TOK_INSTRUCTION,
    TOK_LITERAL,
    TOK_IMMEDIATE_LITERAL,
    TOK_IDENTIFIER,
    TOK_STRING,

    TOK_ERROR,
    TOK_EOF
} token_type;

typedef enum {
    DIR_RESET,
    DIR_BYTE,
    DIR_WORD,
    DIR_STRING,
} directive;

typedef enum {
    INS_LDA,
    INS_LDX,
    INS_LDY,
    INS_STA,
    INS_STX,
    INS_STY,

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

    INS_ADD,
    INS_SUB,
    INS_MUL,
    INS_DIV,
    INS_INA,
    INS_DEA,
    INS_INC,
    INS_DEC,

    INS_JMP,
    INS_JIZ,
    INS_JNZ,
    INS_JIC,
    INS_JNC,
    INS_CLL,
    INS_RET,
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
    } as;
} token;

void init_lexer(const char* source);
token scan_next_token();
