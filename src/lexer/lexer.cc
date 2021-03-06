#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using std::vector;

typedef int (*char_pred)(char c);

typedef struct LexerState {
	const char *source; // The source file
	long int start; // The start index of the current lexeme
	long int current; // The next character to be read from the stream
	long int line; // The current line number in the source file
	long int column; // The column within the current line
	SymbolTable *symbol_table; // The symbol table to add identifiers to
} LexerState;

Token *next_token(LexerState *);

int is_digit(char c) {
	return (c >= '0' && c <= '9');
}

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_alphanum(char c) {
	return is_digit(c) || is_alpha(c);
}

int is_at_end(char c) {
	return c == '\0';
}

char advance(LexerState *state) {
	state->current += 1;
	(state->column)++;
	return state->source[state->current - 1];
}

char peek(LexerState *state) {
	return state->source[state->current];
}

int lookahead_match(LexerState *state, char c) {
	if(peek(state) == c) {
		advance(state);
		return 1;
	} else {
		return 0;
	}
}

int lookahead_pmatch(LexerState *state, char_pred predicate) {
	if(predicate(peek(state))) {
		advance(state);
		return 1;
	} else {
		return 0;
	}
}

Token *character_literal(LexerState *state) {
	char *literal = (char *) malloc(sizeof(char));
	char lexeme = advance(state);
	if(lexeme == '\\') {

		lexeme = advance(state);
		switch(lexeme) {
			case 'a':
				*literal = '\a';
				break;
			case 'b':
				*literal = '\b';
				break;
			case 'f':
				*literal = '\f';
				break;
			case 'n':
				*literal = '\n';
				break;
			case 'r':
				*literal = '\r';
				break;
			case 't':
				*literal = '\t';
				break;
			case 'v':
				*literal = '\v';
				break;
			case '0':
				*literal = '\0';
				break;
		}
		advance(state);
		return create_token(LITERAL, state->line, state->column - 3, L_CHAR, literal, -1);
	} else {
		advance(state);
		*literal = lexeme;
		return create_token(LITERAL, state->line, state->column - 2, L_CHAR, literal, -1);
	}


}

Token *numeric_literal(LexerState *state) {
	int is_real = 0;

	while(lookahead_pmatch(state, is_digit));

	if(lookahead_match(state, '.')) {
		is_real = 1;
	}
	while(lookahead_pmatch(state, is_digit));

	char *lexeme = strndup(state->source + state->start, state->current - state->start);
	long int column = state->column - strlen(lexeme) + 1;
	if(is_real) {
		double *literal = (double *) malloc(sizeof(double));
		*literal = atof(lexeme);
		free(lexeme);
		return create_token(LITERAL, state->line, column, L_DOUBLE, literal, -1);
	} else {
		int *literal = (int *) malloc(sizeof(int));
		*literal = atoi(lexeme);
		free(lexeme);
		return create_token(LITERAL, state->line, column, L_INT, literal, -1);
	}
}

Token *id_or_keyword(LexerState *state) {
	while(lookahead_pmatch(state, is_alphanum));

	char *literal = strndup(state->source + state->start, state->current - state->start);

	long int column = state->column - strlen(literal) + 1;
	Token *token = NULL;
	TokenType tokentype_holder;
	bool is_id = false;
	switch(literal[0]) {
		case 'w' : strcmp(literal, "while") == 0 ? tokentype_holder = WHILE : is_id = true;
				break;
		case 'v' : strcmp(literal, "void") == 0 ? tokentype_holder = VOID : is_id = true;
				break;
		case 'c' : strcmp(literal, "char") == 0 ? tokentype_holder = CHAR : is_id = true;
				break;
		case 'i' : strcmp(literal, "int") == 0 ? tokentype_holder = INT : is_id = true;
				break;
		case 'l' : strcmp(literal, "long") == 0 ? tokentype_holder = LONG : is_id = true;
				break;
		case 'f' : strcmp(literal, "float") == 0 ? tokentype_holder = FLOAT : is_id = true;
				break;
		case 'd' : strcmp(literal, "double") == 0 ? tokentype_holder = DOUBLE : is_id = true;
				break;
		case 'r' : strcmp(literal, "return") == 0 ? tokentype_holder = RETURN : is_id = true;
				break;
		default : is_id = true;
	}

	if(is_id == false) {
		token = create_token(tokentype_holder, state->line, column, (LiteralType) -1,  NULL, -1);
	} else {
		int entry = insertSymbol(state->symbol_table, literal);
		token = create_token(IDENTIFIER, state->line, column, (LiteralType) -1,  NULL, entry);
	}

	free(literal);
	return token;
}

vector<Token *> tokenize(const char *source, SymbolTable *table) {

	LexerState state = {
		.source = source,
		.start = 0,
		.current = 0,
		.line = 1,
		.column = 0,
		.symbol_table = table
	};

	vector<Token *> tokens;
	while(!lookahead_pmatch(&state, is_at_end)) {
		state.start = state.current;
		Token* token = next_token(&state);
		if(token != NULL) {
			tokens.push_back(token);
		}
	}
	tokens.push_back(create_token(TEOF, state.line, state.column, (LiteralType) -1,  NULL, -1));

	return tokens;
}

Token *next_token(LexerState *state) {
	char c = advance(state);
	switch(c) {
		case '\'':
			return character_literal(state);
			break;

		case ';':
			return create_token(SEMI_COLON, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case ',':
			return create_token(COMMA, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case '(':
			return create_token(OPEN_PAREN, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case ')':
			return create_token(CLOSE_PAREN, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '{':
			return create_token(OPEN_BRACE, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '}':
			return create_token(CLOSE_BRACE, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case '[':
			return create_token(OPEN_SQUARE, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case ']':
			return create_token(CLOSE_SQUARE, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case '=':
			return lookahead_match(state, '=') ? create_token(EQUAL_EQUAL, state->line, state->column, (LiteralType) -1,  NULL, -1) : create_token(EQUAL, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case '>':
			return lookahead_match(state, '=') ? create_token(GREATER_EQUAL, state->line, state->column, (LiteralType) -1,  NULL, -1) : create_token(GREATER, state->line, state->column, (LiteralType) -1, NULL, -1);
			break;

		case '<':
			return lookahead_match(state, '=') ? create_token(LESSER_EQUAL, state->line, state->column, (LiteralType) -1,  NULL, -1) : create_token(LESSER, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '&':
			return lookahead_match(state, '&') ? create_token(AND, state->line, state->column, (LiteralType) -1, NULL, -1) : NULL;
			break;

		case '|':
			return lookahead_match(state, '|') ? create_token(OR, state->line, state->column, (LiteralType) -1, NULL, -1) : NULL;
			break;

		case '+':
			return create_token(PLUS, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '-':
			return create_token(MINUS, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '*':
			return create_token(STAR, state->line, state->column, (LiteralType) -1,  NULL, -1);
			break;

		case '/':
			if(lookahead_match(state, '/')) {
				while(!lookahead_pmatch(state, is_at_end));
			} else if(lookahead_match(state, '*')) {
				bool comment_ended = false;
				while(!comment_ended) {
					if(lookahead_match(state, '*')) {
						if(lookahead_match(state, '/')) {
							comment_ended = true;
						}
					}
				}
			}
			else {
				create_token(SLASH, state->line, state->column, (LiteralType) -1,  NULL, -1);
			}
			break;

		case ' ':
		case '\t':
			break;

		case '\n':
			(state->line)++;
			state->column = 0;
			break;

		default:
			if(is_digit(c)) {
				return numeric_literal(state);
			} else if(is_alpha(c)) {
				return id_or_keyword(state);
			}
			break;
	}

	return NULL;
}
