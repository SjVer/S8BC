#pragma once
#define AS_PARSER_H

#include "specs.h"
#include "as/lexer.h"

typedef enum {
	NODE_RAW_DATA,
	NODE_LABEL,
	NODE_INSTRUCTION,
} node_type;

typedef struct {
	int length;
	union {
		byte byte;
		word word;
		byte* array;
	} as;
} raw_data_node;

typedef struct {
	word address;
} label_node;

typedef struct {
	instruction instr;
	bool arg_immediate;
	enum {
		ARG_LABEL,
		ARG_LITERAL
	} arg_kind;
	union {
		char* label;
		word literal;
	} arg_as;
} instruction_node;

typedef struct node {
	node_type type;
	union {
		raw_data_node raw_data;
		label_node label;
		instruction_node instr;
	} as;
	struct node* next;
} node;

void init_parser();
node* parse();