#ifndef CCOMPILER_LEXER_H
#define CCOMPILER_LEXER_H

#include "token.h"

#include <vector>

std::vector<Token *> tokenize(const char *);

#endif
