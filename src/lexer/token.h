#ifndef CCOMPILER_TOKENS_H
#define CCOMPILER_TOKENS_H

typedef enum LiteralType {
	L_CHAR,
	L_INT,
	L_FLOAT,
	L_DOUBLE,
	L_LONG,
	L_STRING
} LiteralType;

typedef enum TokenType {
	// Literals
	LITERAL,

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
	long int line;
	long int column;
	LiteralType ltype;
	void *literal;
	int entry;
	// Additional information pertaining to Token here
} Token;


Token *create_token(TokenType, long int, long int,  LiteralType, void *, int);
void print_token(Token *);

#endif
