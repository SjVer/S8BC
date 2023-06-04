#include <string.h>
#include "as/common.h"
#include "as/lexer.h"

const char* source;
const char* current;
int line, column;

void init_lexer(const char* src) {
	source = src;
	current = source;
	line = 1;
	column = 1;
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

static bool match(char expected)
{
	if (isAtEnd() || *current != expected)
		return false;

	current++;
	return true;
}

static char advance()
{
	current++;
	return current[-1];
}

static char peek()
{
	return *current;
}

static char peek_next()
{
	if (isAtEnd()) return '\0';
	return current[1];
}

static union token_as instr_or_ident()
{
#define Match(str) (memcmp(current, str, strlen(str)))

	if (Match("lda")) return (union token_as){.instruction = }

#undef Match
}

static Token makeToken(token_type type)
{
	Token token;
	token.type = type;
	token.start = start;
	token.length = (int)(current - start);
	token.line = line;
	return token;
}

static Token errorToken(const char *message)
{
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = line;
	return token;
}

static Token string()
{
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n')
			line++;
		advance();
	}

	if (isAtEnd())
		return errorToken("Unterminated string.");

	// The closing quote.
	advance();
	return makeToken(TOKEN_STRING);
}

static Token number()
{
	while (isDigit(peek()))
		advance();

	// Look for a fractional part.
	if (peek() == '.' && isDigit(peek_next()))
	{
		// Consume the ".".
		advance();

		while (isDigit(peek()))
			advance();
	}

	return makeToken(TOKEN_NUMBER);
}

static Token identifier()
{
	while (isAlpha(peek()) || isDigit(peek()))
		advance();
	return makeToken(identifierType());
}

static void skipWhitespace()
{
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
		case '/':
			if (peek_next() == '/')
			{
				// A comment goes until the end of the line.
				while (peek() != '\n' && !isAtEnd())
					advance();
			}
			else
			{
				return;
			}
			break;
		default:
			return;
		}
	}
}

Token scanToken()
{
	skipWhitespace();

	start = current;

	if (isAtEnd())
		return makeToken(TOKEN_EOF);

	char c = advance();

	// check for idents
	if (isAlpha(c))
		return identifier();
	// check for digits
	if (isDigit(c))
		return number();

	switch (c)
	{
	// single-character
	case '(':
		return makeToken(TOKEN_LEFT_PAREN);
	case ')':
		return makeToken(TOKEN_RIGHT_PAREN);
	case '{':
		return makeToken(TOKEN_LEFT_BRACE);
	case '}':
		return makeToken(TOKEN_RIGHT_BRACE);
	case ';':
		return makeToken(TOKEN_SEMICOLON);
	case ',':
		return makeToken(TOKEN_COMMA);
	case '.':
		return makeToken(TOKEN_DOT);
	case '-':
		return makeToken(TOKEN_MINUS);
	case '+':
		return makeToken(TOKEN_PLUS);
	case '/':
		return makeToken(TOKEN_SLASH);
	case '*':
		return makeToken(TOKEN_STAR);

	// two-character
	case '!':
		return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=':
		return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<':
		return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>':
		return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

	// literals
	case '"':
		return string();
	}

	return errorToken("Unexpected character.");
}