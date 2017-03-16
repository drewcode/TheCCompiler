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
void parseVarDecl(ParserState *state);
void parseVarType(ParserState *state);
void parseDecl(ParserState *state);
void parseDeclCond1(ParserState *state);
void parseDeclCond2(ParserState *state);
void parseStmt(ParserState *state);
void parseStmtList(ParserState *state);


void parseExpr1(ParserState *state);
void parseExpr1P(ParserState *state);

void parseExpr2(ParserState *state);
void parseExpr2P(ParserState *state);

void parseExpr3(ParserState *state);
void parseExpr3P(ParserState *state);

void parseExpr4(ParserState *state);
void parseExpr4P(ParserState *state);

void parseExpr5(ParserState *state);
void parseExpr5P(ParserState *state);

void parseExpr6(ParserState *state);
void parseExpr6P(ParserState *state);

void parseExpr7(ParserState *state);


int has_errord(ParserState *state) {
	return state->errord == 1;
}

void errord(ParserState *state) {
    state->errord = 1;
}

Token *next_token(ParserState *state) {
	return state->tokens[(state->current_token)++];
}

int is_token(TokenType type, ParserState *state) {
	Token *token = state->tokens[state->current_token];
	if(token->type == type) {
		return 1;
	}
	return 0;
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
	if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state)) {
		parseVarDecl(state);
		if(state->errord) {
			return;
		}

		parseProgram(state);
	} else if(is_token(IDENTIFIER, state)) {
		next_token(state);
		if(get_token(OPEN_PAREN, state)) {
			if(get_token(CLOSE_PAREN, state)) {
				if(get_token(OPEN_BRACE, state)) {

					parseStmtList(state);
					if(state->errord) {
						return;
					}

					if(get_token(CLOSE_BRACE, state)){
						if(get_token(TEOF, state)) {

						} else {
							// error
							printf("Error: Expected End Of File\n");
							state->errord = 1;
						}
					} else {
						// error
						printf("Error: Expected }\n");
						state->errord = 1;
					}
				} else {
					// error
					printf("Error: Expected {\n");
					state->errord = 1;
				}
			} else {
				// error
				printf("Error: Expected )\n");
				state->errord = 1;
			}
		} else {
			// error
			printf("Error: Expected (\n");
			state->errord = 1;
		}
	} else {
		// error
		printf("Error: Expected Identifier\n");
		state->errord = 1;
	}
}

void parseVarDecl(ParserState *state) {

	parseVarType(state);
	if(state->errord) {
		return;
	}

	parseDecl(state);
	if(state->errord) {
		return;
	}

	if(get_token(SEMI_COLON, state)) {

	} else {
		printf("Error: Expected ;\n");
		state->errord = 1;
	}
}

void parseVarType(ParserState *state) {
	if(get_token(INT, state) || get_token(CHAR, state) || get_token(DOUBLE, state)) {

	} else {
		printf("Error: Expected Variable Type\n");
		state->errord = 1;
	}
}

void parseDecl(ParserState *state) {
	Token *token = get_token(IDENTIFIER, state);
	if(token) {
		parseDeclCond1(state);
		if(state->errord) {

		}
	} else {
		printf("Error: Expected Identifier\n");
		state->errord = 1;
	}
}

void parseDeclCond1(ParserState *state) {
	if(get_token(COMMA, state)) {
		parseDecl(state);
	} else if(get_token(EQUAL, state)){
		parseExpr7(state);

		parseDeclCond2(state);
	} else {
		// epsilon
	}
}

void parseDeclCond2(ParserState *state) {
	if(get_token(COMMA, state)) {
		parseDecl(state);
	} else {
		//epsilon
	}
}

void parseExpr(ParserState *state) {
	if(get_token(IDENTIFIER, state)) {
		if(get_token(EQUAL, state)) {
			parseExpr7(state);
		} else {
			// error
			printf("Error: Expected =\n");
			state->errord = 1;
		}
	} else {
		// error
		state->errord = 1;
	}
}

void parseExpr1(ParserState *state) {
	parseExpr2(state);
	if(state->errord) {
		return;
	}

	parseExpr1P(state);
	if(state->errord) {
		return;
	}
}

void parseExpr1P(ParserState *state) {
	if(get_token(OR, state)) {
		parseExpr2(state);
		if(state->errord) {
			return;
		}

		parseExpr1P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr2(ParserState *state) {
	parseExpr3(state);
	if(state->errord) {
		return;
	}

	parseExpr2P(state);
	if(state->errord) {
		return;
	}
}
void parseExpr2P(ParserState *state) {
	if(get_token(AND, state)) {
		parseExpr3(state);
		if(state->errord) {
			return;
		}

		parseExpr2P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr3(ParserState *state) {
	parseExpr4(state);
	if(state->errord) {
		return;
	}

	parseExpr3P(state);
	if(state->errord) {
		return;
	}
}
void parseExpr3P(ParserState *state) {
	if(get_token(EQUAL_EQUAL, state) || get_token(NOT_EQUAL, state)) {
		parseExpr4(state);
		if(state->errord) {
			return;
		}

		parseExpr3P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr4(ParserState *state) {
	parseExpr5(state);
	if(state->errord) {
		return;
	}

	parseExpr4P(state);
	if(state->errord) {
		return;
	}
}
void parseExpr4P(ParserState *state) {
	if(get_token(GREATER_EQUAL, state) || get_token(LESSER_EQUAL, state)
	|| get_token(GREATER, state) || get_token(LESSER, state)) {
		parseExpr5(state);
		if(state->errord) {
			return;
		}

		parseExpr4P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr5(ParserState *state) {
	parseExpr6(state);
	if(state->errord) {
		return;
	}

	parseExpr5P(state);
	if(state->errord) {
		return;
	}
}
void parseExpr5P(ParserState *state) {
	if(get_token(PLUS, state) || get_token(MINUS, state)) {
		parseExpr6(state);
		if(state->errord) {
			return;
		}

		parseExpr5P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr6(ParserState *state) {
	parseExpr7(state);
	if(state->errord) {
		return;
	}

	parseExpr6P(state);
	if(state->errord) {
		return;
	}
}
void parseExpr6P(ParserState *state) {
	if(get_token(TIMES, state) || get_token(SLASH, state)) {
		parseExpr7(state);
		if(state->errord) {
			return;
		}

		parseExpr6P(state);
		if(state->errord) {
			return;
		}
	} else {
		// epsilon
	}
}

void parseExpr7(ParserState *state) {
	if(get_token(IDENTIFIER, state)) {

	} else if(get_token(LITERAL, state)) {

	} else {
		// error
		printf("Error: Expected Identifier or Literal or Operator\n");
		state->errord = 1;
	}
}

void parseStmt(ParserState *state) {
	if(get_token(SEMI_COLON, state)) {

	} else if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state)) {
		parseVarDecl(state);
	} else if(is_token(IDENTIFIER, state)) {
		parseExpr(state);
		if(get_token(SEMI_COLON, state)) {

		} else {
			// error
			printf("Error: Expected ;\n");
			state->errord = 1;
		}

	} else if(get_token(RETURN, state)) {
		parseExpr7(state);
		if(get_token(SEMI_COLON, state)) {

		} else {
			// error
			printf("Error: Expected ;\n");
			state->errord = 1;
		}
	} else if(get_token(WHILE, state)) {
		if(get_token(OPEN_PAREN, state)) {

			parseExpr7(state);
			if(state->errord == 1) {
				return ;
			}

			if(get_token(CLOSE_PAREN, state)) {

				if(get_token(OPEN_BRACE, state)) {

					parseStmtList(state);
					if(state->errord == 1) {
						return;
					}

					if(get_token(CLOSE_BRACE, state)) {

					} else {
						// error
						printf("Error: Expected }\n");
						state->errord = 1;
					}

				} else {
					// error
					printf("Error: Expected {\n");
					state->errord = 1;
				}

			} else {
				// error
				printf("Error: Expected )\n");
				state->errord = 1;
			}
		} else {
			//error
			printf("Error: Expected (\n");
			state->errord = 1;
		}
	} else {
		// error
	}
}

void parseStmtList(ParserState *state) {
	parseStmt(state);

	if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state) || is_token(SEMI_COLON, state)
		|| is_token(WHILE, state) || is_token(IDENTIFIER, state) || is_token(RETURN, state)) {
		parseStmt(state);
		if(state->errord == 1) {
			return;
		}

		parseStmtList(state);
	} else {
		// epsilon
	}
}
