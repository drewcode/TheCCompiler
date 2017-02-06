#include "token.h"

#include <stdlib.h>
#include <stdio.h>

Token *create_token(TokenType type, void *literal) {
	Token *token = (Token *) malloc(sizeof(Token));
	if(token == NULL) {
		return NULL;
	}

	token->type = type;
	token->literal = literal;

	return token;
}

void print_token(Token *token) {
	switch(token->type) {
		case IDENTIFIER:
			printf("Identifier: %s\n", (char *)token->literal);
			break;

		case SEMI_COLON:
			printf("Semi Colon\n");
			break;

		case COMMA:
			printf("Comma\n");
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
		case VOID:
		case CHAR:
		case INT:
		case LONG:
		case DOUBLE:
		case FLOAT:
		case RETURN:
			printf("Keyword: %s\n", (char *)token->literal);
			break;

		case INT_LITERAL:
			printf("Integer Literal: %d\n", *((int *)token->literal));
			break;

		case DOUBLE_LITERAL:
			printf("Double Literal: %f\n", *((double *)token->literal));
			break;

		default:
			printf("Some Other Token\n");
	}
}
