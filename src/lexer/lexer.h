#ifndef CCOMPILER_LEXER_H
#define CCOMPILER_LEXER_H

#include "token.h"
#include "../symbol_table/sym_tab.h"

#include <vector>

std::vector<Token *> tokenize(const char *, SymbolTable *);

#endif
