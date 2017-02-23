#include "token.h"

#include <stdlib.h>
#include <stdio.h>

Token *create_token(TokenType type, long int line, long int column, LiteralType ltype, void *literal, int entry) {
	Token *token = (Token *) malloc(sizeof(Token));
	if(token == NULL) {
		return NULL;
	}

	token->type = type;
	token->line = line;
	token->column = column;
	token->ltype = ltype;
	token->literal = literal;
	token->entry  = entry;

	return token;
}

void print_token(Token *token) {
	printf("Line: %ld Column: %ld ", token->line, token->column);
	switch(token->type) {
		case IDENTIFIER:
			printf("Identifier: %d\n", token->entry);
			break;

		case SEMI_COLON:
			printf("Semi Colon\n");
			break;

		case COMMA:
			printf("Comma\n");
			break;

		case EQUAL:
			printf("Equal\n");
			break;

		case OPEN_BRACE:
			printf("Open Braces\n");
			break;

		case CLOSE_BRACE:
			printf("Close Braces\n");
			break;

		case OPEN_PAREN:
			printf("Open Parenthesis\n");
			break;

		case CLOSE_PAREN:
			printf("Close Parenthesis\n");
			break;

		case OPEN_SQUARE:
			printf("Open Square Brackets\n");
			break;

		case CLOSE_SQUARE:
			printf("Close Square Brackets\n");
			break;

		case WHILE:
			printf("Keyword: while\n");
			break;
		case VOID:
			printf("Keyword: void\n");
			break;
		case CHAR:
			printf("Keyword: char\n");
			break;
		case INT:
			printf("Keyword: int\n");
			break;
		case LONG:
			printf("Keyword: long\n");
			break;
		case DOUBLE:
			printf("Keyword: double\n");
			break;
		case FLOAT:
			printf("Keyword: float\n");
			break;
		case RETURN:
			printf("Keyword: return\n");
			break;

		case LITERAL:
			if(token->ltype == L_INT) {
				printf("Integer Literal: %d\n", *((int *)token->literal));
			} else if(token->ltype == L_DOUBLE) {
				printf("Double Literal: %f\n", *((double *)token->literal));
			} else if(token->ltype == L_CHAR) {
				printf("Character Literal: '%c'\n", *((char *)token->literal));
			}
			break;

		default:
			printf("Some Other Token\n");
	}
}
