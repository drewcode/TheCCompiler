#include "../lexer/token.h"
#include "../symbol_table/sym_tab.h"
#include "parser.h"

#include <stdio.h>
#include <vector>

using std::vector;

typedef struct ParserState {
	vector<Token *> tokens;
	int current_token;
	SymbolTable *symbol_table;
	int errord;
} ParserState;

void parseProgram(ParserState *state);
void parseFunctionDeclarationParameterListStart(ParserState *state);
void parseFunctionDeclarationParameterListMore(ParserState *state);
void parseParameterDataType(ParserState *state);

int has_errord(ParserState *state) {
	return state->errord == 1;
}

void errord(ParserState *state) {
    state->errord = 1;
}

Token *next_token(ParserState *state) {
	return state->tokens[(state->current_token)++];
}

Token *get_token(TokenType type, ParserState *state) {
    Token *token = state->tokens[state->current_token];
    if(token->type == type) {
        return state->tokens[(state->current_token)++];
    } else {
        return NULL;
    }
}

void runParser(vector<Token *> *tokensPtr, SymbolTable *table) {
	ParserState state = {
		.tokens = *tokensPtr,
		.current_token = 0,
		.symbol_table = table,
		.errord = 0
	};

	parseProgram(&state);
    if(state.current_token == state.tokens.size()) {
		printf("Parse Successful\n");
	}
}

void parseProgram(ParserState *state) {
    if(get_token(VOID, state) != NULL
    || get_token(CHAR, state) != NULL
    || get_token(INT, state) != NULL
    || get_token(DOUBLE, state) != NULL
    ) {
        if(get_token(IDENTIFIER, state)) {
            if(get_token(OPEN_PAREN, state)) {
                parseFunctionDeclarationParameterListStart(state);
                if(!has_errord(state)) {
                    if(get_token(CLOSE_PAREN, state)) {
                        if(get_token(SEMI_COLON, state)) {
                            parseProgram(state);
                        } else {
                            printf("Error: Expected ;\n");
                            errord(state);
                        }
                    } else {
                        printf("Error: Expected )\n");
                        errord(state);
                    }
                }
            } else {
                printf("Error: Expected (\n");
                errord(state);
            }
        } else {
            printf("Error: Expected Identifier\n");
            errord(state);
        }
    } else if(get_token(TEOF, state)) {

    } else {
        printf("Error: Expected Return Type\n");
        errord(state);
    }
}

void parseFunctionDeclarationParameterListStart(ParserState *state) {
    if( get_token(CHAR, state) != NULL
    || get_token(INT, state) != NULL
    || get_token(DOUBLE, state) != NULL
    ) {
        if(get_token(IDENTIFIER, state)) {
            parseFunctionDeclarationParameterListMore(state);
        } else {
            printf("Error: Expected Identifier\n");
            errord(state);
        }
    } else {
        // epsilon
    }
}

void parseFunctionDeclarationParameterListMore(ParserState *state) {
    if(get_token(COMMA, state)) {
        parseParameterDataType(state);
        if(!has_errord(state)) {
            if(get_token(IDENTIFIER, state)) {
                parseFunctionDeclarationParameterListMore(state);
            } else {
                printf("Error: Expected Identifier\n");
                errord(state);
            }
        }
    } else {
        // epsilon
    }
}

void parseParameterDataType(ParserState *state) {
    if( get_token(CHAR, state) != NULL
    || get_token(INT, state) != NULL
    || get_token(DOUBLE, state) != NULL
    ) {

    } else {
        printf("Error: Expected Data Type\n");
        errord(state);
    }
}
