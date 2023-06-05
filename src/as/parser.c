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

	for (;;) {
		curr_token = scan_next_token();
		if (curr_token.type != TOK_ERROR)
			error_at(&curr_token, curr_token.as.error);
		else break;
	}
}

static bool check(token_type type) {
	return curr_token.type == type;
}

static bool match(token_type type) {
	if (!check(type)) return false;
	advance();
	return true;
}

static void consume(token_type type) {
	if (!match(type))
		error_at(&curr_token, "unexpected token");
}

static long parse_literal(bool immediate) {
	if (immediate) consume(TOK_IMMEDIATE_LITERAL);
	else consume(TOK_LITERAL);

	const char* start = prev_token.start;
	if (immediate) start += 1;

	// safe thanks to the lexer
	return strtoul(start, NULL, 10);
}

static node* parse_directive() {
	switch (prev_token.as.dir) {
		case DIR_RESET: {
			error_at(&prev_token, "not yet implemented");
		}

		case DIR_BYTE: {
			long l = parse_literal(false);
			if (l > 0xff) error_at(&prev_token, "value too large");

			node* n = malloc(sizeof(node));
			n->type = NODE_RAW_DATA;
			n->as.raw_data.length = 1;
			n->as.raw_data.as.byte = l;
		}
		case DIR_WORD: {
			long l = parse_literal(false);
			if (l > 0xffff) error_at(&prev_token, "value too large");

			node* n = malloc(sizeof(node));
			n->type = NODE_RAW_DATA;
			n->as.raw_data.length = 2;
			n->as.raw_data.as.word = l;
		}
		case DIR_STRING: {
			consume(TOK_STRING);
			char* str = malloc(prev_token.length - 2);
			strncpy(str, prev_token.start + 1, prev_token.length - 2);

			node* n = malloc(sizeof(node));
			n->type = NODE_RAW_DATA;
			n->as.raw_data.length = prev_token.length - 2;
			n->as.raw_data.as.array = (byte*)str;
		}
	}
}

static node* parse_node() {
	if (match(TOK_DIRECTIVE))
		return parse_directive();

	else if (match(TOK_INSTRUCTION))
		return parse_instruction();

	else if (match(TOK_IDENTIFIER) || match(TOK_LITERAL))
		return parse_label();

	else error_at(&curr_token, "unexpected token");
}

node* parse() {
	// sets up curr_token
	advance();
	
	node* first_node = NULL;
	node* prev_node = NULL;
	
	while (!match(TOK_EOF)) {
		node* next_node = parse_node();

		if (prev_node) prev_node->next = next_node;
		else first_node = next_node;
		prev_node = next_node;
	}

	return first_node;
}