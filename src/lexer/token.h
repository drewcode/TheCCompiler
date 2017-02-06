#ifndef CCOMPILER_TOKENS_H
#define CCOMPILER_TOKENS_H

typedef enum TokenType {
	// Literals
	INT_LITERAL,
	DOUBLE_LITERAL,

	IDENTIFIER,

	// Keywords
	VOID,
	CHAR,
	INT,
	LONG,
	FLOAT,
	DOUBLE,
	RETURN,
	WHILE,

	// Seperators
	SEMI_COLON,
	COMMA,
	OPEN_BRACE,
	CLOSE_BRACE,
	OPEN_PAREN,
	CLOSE_PAREN,
	OPEN_SQUARE,
	CLOSE_SQUARE,

	// Operators
	EQUAL,
	EQUAL_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESSER,
	LESSER_EQUAL,
	NOT_EQUAL,
} TokenType;

typedef struct Token {
	TokenType type;
	void *literal;
	// Additional information pertaining to Token here
} Token;


Token *create_token(TokenType, void *);
void print_token(Token *);

#endif
