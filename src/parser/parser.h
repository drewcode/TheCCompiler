#ifndef CCOMPILER_PARSER_H
#define CCOMPILER_PARSER_H

#include "../lexer/token.h"
#include "../symbol_table/sym_tab.h"
#include <stdio.h>
#include <vector>

void runParser(std::vector<Token *> *, SymbolTable *table);

#endif
