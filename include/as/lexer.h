#pragma once
#define AS_LEXER_H

typedef enum {
	TOK_DIRECTIVE,
	TOK_LABEL,
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
	DIR_AT,
	DIR_BYTE,
	DIR_WORD,
} directive;

typedef enum {
	INS_LDA,
	INS_LDX,
	INS_LDY,
	INS_STA,
	INS_STX,
	INS_STY,

    OP_TAX,
    OP_TAY,
    OP_TXA,
    OP_TYA,
    OP_SAX,
    OP_SAY,
    OP_SXY,

    OP_TSX,
    OP_TXS,
    OP_PSH,
    OP_PLL,
    OP_POP,

    OP_AND,
    OP_IOR,
    OP_XOR,
    OP_SHL,
    OP_SHR,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_INA,
    OP_DEA,
    OP_INC,
    OP_DEC,

    OP_JMP,
    OP_JIZ,
    OP_JNZ,
    OP_JIC,
    OP_JNC,
    OP_CLL,
    OP_RET,
    OP_HLT,
} instruction;

typedef struct {
	token_type type;
	const char* start;
	int line, column, length;
	
	union token_as {
		directive directive;
		instruction instruction;
		int integer;
		char* string;
	} as;
} token;

void init_lexer(const char* source);
token next_token();
