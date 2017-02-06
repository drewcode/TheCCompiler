#include "token.h"
#include <vector>
#include <stdlib.h>
#include <string.h>

using std::vector;

typedef int (*char_pred)(char c);



Token *next_token(const char *, long int *, long int);

int is_digit(char c) {
	return (c >= '0' && c <= '9');
}

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_alphanum(char c) {
	return is_digit(c) || is_alpha(c);
}

int is_at_end(const char *source, long int current) {
	return source[current] == '\0';
}

char advance(const char *source, long int *current) {
	*current += 1;
	return source[*current - 1];
}

char peek(const char *source, long int current) {
	return source[current];
}

int lookahead_match(const char *source, long int *current, char c) {
	if(peek(source, *current) == c) {
		advance(source, current);
		return 1;
	} else {
		return 0;
	}
}

int lookahead_pmatch(const char *source, long int *current, char_pred predicate) {
	if(predicate(peek(source, *current))) {
		advance(source, current);
		return 1;
	} else {
		return 0;
	}
}

Token *numeric(const char *source, long int *current, long int start) {
	int is_real = 0;

	while(lookahead_pmatch(source, current, is_digit));
	if(lookahead_match(source, current, '.')) {
		is_real = 1;
	}
	while(lookahead_pmatch(source, current, is_digit));

	char *lexeme = strndup(source + start, *current - start);

	if(is_real) {
		double *literal = (double *) malloc(sizeof(double));
		*literal = atof(lexeme);
		return create_token(DOUBLE_LITERAL, literal);
	} else {
		int *literal = (int *) malloc(sizeof(int));
		*literal = atoi(lexeme);
		return create_token(INT_LITERAL, literal);
	}
}

Token *id_or_keyword(const char *source, long int *current, long int start) {
	while(lookahead_pmatch(source, current, is_alphanum));

	char *literal = strndup(source + start, *current - start);

	//TODO: Make this logic more efficient
	if(strcmp(literal, "while") == 0) {
		return create_token(WHILE, literal);
	} else if(strcmp(literal, "void") == 0) {
		return create_token(VOID, literal);
	} else if(strcmp(literal, "char") == 0) {
		return create_token(CHAR, literal);
	} else if(strcmp(literal, "int") == 0) {
		return create_token(INT, literal);
	} else if(strcmp(literal, "long") == 0) {
		return create_token(LONG, literal);
	} else if(strcmp(literal, "float") == 0) {
		return create_token(FLOAT, literal);
	} else if(strcmp(literal, "double") == 0) {
		return create_token(DOUBLE, literal);
	} else if(strcmp(literal, "return") == 0) {
		return create_token(RETURN, literal);
	} else {
		return create_token(IDENTIFIER, literal);
	}
}

vector<Token *> tokenize(const char *source) {



	long int start = 0;
	long int current = 0;
	long int line = 1;
	vector<Token *> tokens;
	while(!is_at_end(source, current)) {
		start = current;
		Token* token = next_token(source, &current, start);
		if(token != NULL) {
			tokens.push_back(token);
		}
	}

	return tokens;
}

Token *next_token(const char *source, long int *current, long int start) {
	char c = advance(source, current);
	switch(c) {
		case ';':
			return create_token(SEMI_COLON, NULL);
			break;

		case ',':
			return create_token(COMMA, NULL);
			break;

		case '(':
			return create_token(OPEN_PAREN, NULL);
			break;

		case ')':
			return create_token(CLOSE_PAREN, NULL);
			break;

		case '{':
			return create_token(OPEN_BRACE, NULL);
			break;

		case '}':
			return create_token(CLOSE_BRACE, NULL);
			break;

		case '[':
			return create_token(OPEN_SQUARE, NULL);
			break;

		case ']':
			return create_token(CLOSE_SQUARE, NULL);
			break;

		case '=':
			return lookahead_match(source, current, '=') ? create_token(EQUAL_EQUAL, NULL) : create_token(EQUAL, NULL);
			break;

		case '>':
			return lookahead_match(source, current, '=') ? create_token(GREATER_EQUAL, NULL) : create_token(GREATER, NULL);
			break;

		case '<':
			return lookahead_match(source, current, '=') ? create_token(LESSER_EQUAL, NULL) : create_token(LESSER, NULL);
			break;

		case ' ':
		case '\t':
		case '\n':
			break;

		default:
			if(is_digit(c)) {
				return numeric(source, current, start);
			} else if(is_alpha(c)) {
				return id_or_keyword(source, current, start);
			}
			break;
	}

	return NULL;
}
