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
	bool is_ident;
	bool is_array;
	int length;
	union {
		char* ident;
		byte byte;
		word word;
		byte* array;
	} as;
} raw_data_node;

typedef struct {
	bool is_ident;
	union {
		word literal;
		char* ident;
	} as;
} label_node;

typedef struct {
	instruction instr;
	token_type arg_type;
	union {
		char* ident;
		byte imm_literal;
		word literal;
	} as;
} instr_node;

typedef struct node {
	word address;
	node_type type;
	union {
		raw_data_node raw_data;
		label_node label;
		instr_node instr;
	} as;
	struct node* next;
} node;

void init_parser();
node* parse();