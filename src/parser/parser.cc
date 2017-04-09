#include "ast.h"
#include "parser.h"
#include "../lexer/token.h"

using std::vector;

typedef struct ParserState {
	vector<Token *> tokens;
	int current_token;
	SymbolTable *symbol_table;
	int errord;
} ParserState;

ASTProgram *parseProgram(ParserState *state, vector<ASTVariableDeclarationStatement *> *inhDeclList);
ASTVariableDeclarationStatement *parseVarDecl(ParserState *);
TokenType parseVarType(ParserState *state);
vector<ASTVariableDeclarator *> *parseDecl(ParserState *, TokenType, vector<ASTVariableDeclarator *> *);
vector<ASTVariableDeclarator *> *parseDeclContd1(ParserState *state, TokenType, vector<ASTVariableDeclarator *> *, ASTIndentifier *);
vector<ASTVariableDeclarator *> *parseDeclContd2(ParserState *state, TokenType, vector<ASTVariableDeclarator *> *);

ASTStatement *parseStmt(ParserState *state);
vector<ASTStatement *> *parseStmtList(ParserState *state, vector<ASTStatement *> *);

ASTAssignmentExpression *parseExpr(ParserState *state);

ASTExpression *parseExpr1(ParserState *state);
ASTExpression *parseExpr1P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr2(ParserState *state);
ASTExpression *parseExpr2P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr3(ParserState *state);
ASTExpression *parseExpr3P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr4(ParserState *state);
ASTExpression *parseExpr4P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr5(ParserState *state);
ASTExpression *parseExpr5P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr6(ParserState *state);
ASTExpression *parseExpr6P(ParserState *state, ASTExpression *);

ASTExpression *parseExpr7(ParserState *state);


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

	ASTProgram *program = parseProgram(&state, new vector<ASTVariableDeclarationStatement *>());
    if(state.current_token == state.tokens.size()) {
    	FILE *icg_f = fopen("test.icg", "w");
		printf("Parse Successful\n");
		program->print(0, icg_f);
	}
}

ASTProgram *parseProgram(ParserState *state, vector<ASTVariableDeclarationStatement *> *inhDeclList) {
	if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state)) {
		ASTVariableDeclarationStatement *vardecl = parseVarDecl(state);
		if(state->errord) {
			return NULL;
		}
		inhDeclList->push_back(vardecl);
		ASTProgram *program = parseProgram(state, inhDeclList);
		// error
		if(state->errord) {
			return NULL;
		}

		return program;
	} else if(is_token(IDENTIFIER, state)) {
		next_token(state);
		if(get_token(OPEN_PAREN, state)) {
			if(get_token(CLOSE_PAREN, state)) {
				if(get_token(OPEN_BRACE, state)) {

					vector<ASTStatement *> *body = parseStmtList(state, new vector<ASTStatement *>());
					if(state->errord) {
						return NULL;
					}

					if(get_token(CLOSE_BRACE, state)){
						if(get_token(TEOF, state)) {
							return new ASTProgram(inhDeclList, body);
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

	return NULL;
}

ASTVariableDeclarationStatement *parseVarDecl(ParserState *state) {
	TokenType type = parseVarType(state);
	if(state->errord) {
		return NULL;
	}

	vector<ASTVariableDeclarator *> * decls = parseDecl(state, type, new vector<ASTVariableDeclarator *>());
	if(state->errord) {
		return NULL;
	}

	if(get_token(SEMI_COLON, state)) {
		return new ASTVariableDeclarationStatement(type, decls);
	} else {
		printf("Error: Expected ;\n");
		state->errord = 1;
	}

	return NULL;
}

TokenType parseVarType(ParserState *state) {
	if(is_token(INT, state) || is_token(CHAR, state) || is_token(DOUBLE, state)) {
		return next_token(state)->type;
	} else {
		printf("Error: Expected Variable Type\n");
		state->errord = 1;
	}
}

vector<ASTVariableDeclarator *> *parseDecl(ParserState *state, TokenType inhType, vector<ASTVariableDeclarator *> *inhList) {
	if(is_token(IDENTIFIER, state)) {
		Token *id_token = next_token(state);
		ASTIndentifier *id =  new ASTIndentifier(id_token->entry);
		vector<ASTVariableDeclarator *> *list = parseDeclContd1(state, inhType, inhList, id);
		if(state->errord) {
			return NULL;
		}

		return list;
	} else {
		printf("Error: Expected Identifier\n");
		state->errord = 1;
	}

	return NULL;
}

vector<ASTVariableDeclarator *> *parseDeclContd1(ParserState *state, TokenType inhType, vector<ASTVariableDeclarator *> *inhList, ASTIndentifier *inhId) {

	if(get_token(COMMA, state)) {
		inhList->push_back(new ASTVariableDeclarator(inhId, NULL));
		vector<ASTVariableDeclarator *> *list = parseDecl(state, inhType, inhList);
		if(state->errord) {
			return NULL;
		}

		return list;
	} else if(get_token(EQUAL, state)){
		ASTExpression *expr1 = parseExpr1(state);
		if(state->errord) {
			return NULL;
		}
		inhList->push_back(new ASTVariableDeclarator(inhId, expr1));
		vector<ASTVariableDeclarator *> *list = parseDeclContd2(state, inhType, inhList);
		if(state->errord) {
			return NULL;
		}
		return list;
	} else {
		// epsilon
		inhList->push_back(new ASTVariableDeclarator(inhId, NULL));
		return inhList;
	}
}

vector<ASTVariableDeclarator *> *parseDeclContd2(ParserState *state, TokenType inhType, vector<ASTVariableDeclarator *> *inhList) {
	if(get_token(COMMA, state)) {
		vector<ASTVariableDeclarator *> *list = parseDecl(state, inhType, inhList);
		if(state->errord) {
			return NULL;
		}

		return list;
	} else {
		//epsilon
		return inhList;
	}
}

ASTAssignmentExpression *parseExpr(ParserState *state) {
	if(is_token(IDENTIFIER, state)) {
		Token *id_t = next_token(state);
		if(get_token(EQUAL, state)) {
			ASTExpression *expr1 = parseExpr1(state);
			if(state->errord) {
				return NULL;
			}
			ASTIndentifier *id = new ASTIndentifier(id_t->entry);
			return new ASTAssignmentExpression(id, expr1);
		} else {
			// error
			printf("Error: Expected =\n");
			state->errord = 1;
		}
	} else {
		// error
		printf("Error: Expected Identifier\n");
		state->errord = 1;
	}

	return NULL;
}

ASTExpression *parseExpr1(ParserState *state) {
	ASTExpression * expr2 = parseExpr2(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr1p = parseExpr1P(state, expr2);
	if(state->errord) {
		return NULL;
	}

	return expr1p;
}

ASTExpression *parseExpr1P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(OR, state)) {
		Token *op = next_token(state);
		ASTExpression *expr2 = parseExpr2(state);
		if(state->errord) {
			return NULL;
		}

		ASTExpression *res = new ASTBinaryExpression(op->type, inhNode, expr2);

		ASTExpression *expr1p = parseExpr1P(state, res);
		if(state->errord) {
			return NULL;
		}

		return expr1p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr2(ParserState *state) {
	ASTExpression *expr3 = parseExpr3(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr2p = parseExpr2P(state, expr3);
	if(state->errord) {
		return NULL;
	}
	return expr2p;
}
ASTExpression *parseExpr2P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(AND, state)) {
		Token *op = next_token(state);
		ASTExpression *expr3 = parseExpr3(state);

		if(state->errord) {
			return NULL;
		}
		ASTExpression *res = new ASTBinaryExpression(op->type, inhNode, expr3);

		ASTExpression *expr2p = parseExpr2P(state, res);
		if(state->errord) {
			return NULL;
		}
		return expr2p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr3(ParserState *state) {
	ASTExpression *expr4 = parseExpr4(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr3p = parseExpr3P(state, expr4);
	if(state->errord) {
		return NULL;
	}
	return expr3p;
}

ASTExpression *parseExpr3P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(EQUAL_EQUAL, state) || is_token(NOT_EQUAL, state)) {
		Token *op = next_token(state);

		ASTExpression *expr4 = parseExpr4(state);
		if(state->errord) {
			return NULL;
		}
		ASTExpression *res = (op->type, inhNode, expr4);

		ASTExpression *expr3p = parseExpr3P(state, res);
		if(state->errord) {
			return NULL;
		}
		return expr3p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr4(ParserState *state) {
	ASTExpression *expr5 = parseExpr5(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr4p = parseExpr4P(state, expr5);
	if(state->errord) {
		return NULL;
	}
	return expr4p;
}
ASTExpression *parseExpr4P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(GREATER_EQUAL, state) || is_token(LESSER_EQUAL, state)
	|| is_token(GREATER, state) || is_token(LESSER, state)) {
		Token *op = next_token(state);
		ASTExpression *expr5 = parseExpr5(state);
		if(state->errord) {
			return NULL;
		}

		ASTExpression *res = new ASTBinaryExpression(op->type, inhNode, expr5);
		ASTExpression *expr4p = parseExpr4P(state, res);
		if(state->errord) {
			return NULL;
		}
		return expr4p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr5(ParserState *state) {
	ASTExpression *expr6 = parseExpr6(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr5p = parseExpr5P(state, expr6);
	if(state->errord) {
		return NULL;
	}

	return expr5p;
}
ASTExpression *parseExpr5P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(PLUS, state) || is_token(MINUS, state)) {
		Token *op = next_token(state);
		ASTExpression *expr6 = parseExpr6(state);
		if(state->errord) {
			return NULL;
		}

		ASTExpression *res = new ASTBinaryExpression(op->type, inhNode, expr6);

		ASTExpression *expr5p = parseExpr5P(state, res);
		if(state->errord) {
			return NULL;
		}

		return expr5p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr6(ParserState *state) {
	ASTExpression *expr7 = parseExpr7(state);
	if(state->errord) {
		return NULL;
	}

	ASTExpression *expr6p = parseExpr6P(state, expr7);
	if(state->errord) {
		return NULL;
	}

	return expr6p;
}

ASTExpression *parseExpr6P(ParserState *state, ASTExpression *inhNode) {
	if(is_token(STAR, state) || is_token(SLASH, state)) {
		Token *op = next_token(state);
		ASTExpression *expr7 = parseExpr7(state);
		if(state->errord) {
			return NULL;
		}

		ASTBinaryExpression *res = new ASTBinaryExpression(op->type, inhNode, expr7);

		ASTExpression *expr6p = parseExpr6P(state, res);
		if(state->errord) {
			return NULL;
		}
		return expr6p;
	} else {
		// epsilon
		return inhNode;
	}
}

ASTExpression *parseExpr7(ParserState *state) {
	if(is_token(IDENTIFIER, state)) {
		Token *id_token = get_token(IDENTIFIER, state);
		return new ASTIndentifier(id_token->entry);
	} else if(is_token(LITERAL, state)) {
		Token *literal_token = get_token(LITERAL, state);
		return new ASTLiteral(literal_token->ltype, literal_token->literal);
	} else if(get_token(OPEN_PAREN, state)) {
		ASTExpression *expr = parseExpr1(state);
		if(state->errord) {
			return NULL;
		}

		if(get_token(CLOSE_PAREN, state)) {
			return expr;
		} else {
			// error
			printf("Error: Expected )\n");
			state->errord = 1;
		}
	} else {
		// error
		printf("Error: Expected Identifier or Literal or Operator\n");
		state->errord = 1;
	}

	return NULL;
}

ASTStatement *parseStmt(ParserState *state) {
	if(get_token(SEMI_COLON, state)) {
		return new ASTEmptyStatement();
	} else if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state)) {
		ASTStatement *stmt = parseVarDecl(state);
		if(state->errord) {
			return NULL;
		}

		return stmt;
	} else if(is_token(IDENTIFIER, state)) {

		ASTAssignmentExpression *expr = parseExpr(state);
		if(state->errord) {
			return NULL;
		}
		if(get_token(SEMI_COLON, state)) {
			return new ASTAssignmentStatement(expr);
		} else {
			// error
			printf("Error: Expected ;\n");
			state->errord = 1;
		}

	} else if(get_token(RETURN, state)) {
		ASTExpression *expr1 = parseExpr1(state);
		if(get_token(SEMI_COLON, state)) {
			return new ASTReturnStatement(expr1);
		} else {
			// error
			printf("Error: Expected ;\n");
			state->errord = 1;
		}
	} else if(get_token(WHILE, state)) {
		if(get_token(OPEN_PAREN, state)) {

			ASTExpression *expr1 = parseExpr1(state);
			if(state->errord == 1) {
				return NULL;
			}

			if(get_token(CLOSE_PAREN, state)) {

				if(get_token(OPEN_BRACE, state)) {

					vector<ASTStatement *> *stmtl = parseStmtList(state, new vector<ASTStatement *>());
					if(state->errord == 1) {
						return NULL;
					}

					if(get_token(CLOSE_BRACE, state)) {
						return new ASTWhileStatement(expr1, stmtl);
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
		printf("Error: OMG WTF\n");
		state->errord = 1;
	}

	return NULL;
}

vector<ASTStatement *> *parseStmtList(ParserState *state, vector<ASTStatement *> *inhList) {
	if(is_token(CHAR, state) || is_token(INT, state) || is_token(DOUBLE, state) || is_token(SEMI_COLON, state)
		|| is_token(WHILE, state) || is_token(IDENTIFIER, state) || is_token(RETURN, state)) {
		ASTStatement *stmt = parseStmt(state);
		if(state->errord == 1) {
			return NULL;
		}
		inhList->push_back(stmt);
		return parseStmtList(state, inhList);
	} else {
		// epsilon
		return inhList;
	}
}
