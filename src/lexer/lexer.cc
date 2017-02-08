#include "token.h"
#include <vector>
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

int is_at_end(LexerState *state) {
	return state->source[state->current] == '\0';
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

Token *numeric(LexerState *state) {
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
		return create_token(DOUBLE_LITERAL, state->line, column, literal);
	} else {
		int *literal = (int *) malloc(sizeof(int));
		*literal = atoi(lexeme);
		free(lexeme);
		return create_token(INT_LITERAL, state->line, column, literal);
	}
}

Token *id_or_keyword(LexerState *state) {
	while(lookahead_pmatch(state, is_alphanum));

	char *literal = strndup(state->source + state->start, state->current - state->start);

	//TODO: Make this logic more efficient
	long int column = state->column - strlen(literal) + 1;
	Token *token = NULL;
	if(strcmp(literal, "while") == 0) {
		token = create_token(WHILE, state->line, column, NULL);
	} else if(strcmp(literal, "void") == 0) {
		token = create_token(VOID, state->line, column, NULL);
	} else if(strcmp(literal, "char") == 0) {
		token = create_token(CHAR, state->line, column, NULL);
	} else if(strcmp(literal, "int") == 0) {
		token = create_token(INT, state->line, column, NULL);
	} else if(strcmp(literal, "long") == 0) {
		token = create_token(LONG, state->line, column, NULL);
	} else if(strcmp(literal, "float") == 0) {
		token = create_token(FLOAT, state->line, column, NULL);
	} else if(strcmp(literal, "double") == 0) {
		token = create_token(DOUBLE, state->line, column, NULL);
	} else if(strcmp(literal, "return") == 0) {
		token = create_token(RETURN, state->line, column, NULL);
	}

	if(token == NULL) {
		token = create_token(IDENTIFIER, state->line, column, literal);
	} else {
		free(literal);
	}

	return token;
}

vector<Token *> tokenize(const char *source) {

	LexerState state = {
		.source = source,
		.start = 0,
		.current = 0,
		.line = 1,
		.column = 0
	};

	vector<Token *> tokens;
	while(!is_at_end(&state)) {
		state.start = state.current;
		Token* token = next_token(&state);
		if(token != NULL) {
			tokens.push_back(token);
		}
	}

	return tokens;
}

Token *next_token(LexerState *state) {
	char c = advance(state);
	switch(c) {
		case ';':
			return create_token(SEMI_COLON, state->line, state->column, NULL);
			break;

		case ',':
			return create_token(COMMA, state->line, state->column, NULL);
			break;

		case '(':
			return create_token(OPEN_PAREN, state->line, state->column, NULL);
			break;

		case ')':
			return create_token(CLOSE_PAREN, state->line, state->column, NULL);
			break;

		case '{':
			return create_token(OPEN_BRACE, state->line, state->column, NULL);
			break;

		case '}':
			return create_token(CLOSE_BRACE, state->line, state->column, NULL);
			break;

		case '[':
			return create_token(OPEN_SQUARE, state->line, state->column, NULL);
			break;

		case ']':
			return create_token(CLOSE_SQUARE, state->line, state->column, NULL);
			break;

		case '=':
			return lookahead_match(state, '=') ? create_token(EQUAL_EQUAL, state->line, state->column, NULL) : create_token(EQUAL, state->line, state->column, NULL);
			break;

		case '>':
			return lookahead_match(state, '=') ? create_token(GREATER_EQUAL, state->line, state->column, NULL) : create_token(GREATER, state->line, state->column, NULL);
			break;

		case '<':
			return lookahead_match(state, '=') ? create_token(LESSER_EQUAL, state->line, state->column, NULL) : create_token(LESSER, state->line, state->column, NULL);
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
				return numeric(state);
			} else if(is_alpha(c)) {
				return id_or_keyword(state);
			}
			break;
	}

	return NULL;
}
