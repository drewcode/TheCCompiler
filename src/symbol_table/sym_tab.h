#ifndef CCOMPILER_SYM_TAB_H
#define CCOMPILER_SYM_TAB_H

#include <vector>

typedef struct SymbolTableEntry {
	char *name;
} SymbolTableEntry;

typedef std::vector<SymbolTableEntry *> SymbolTable;

int insertSymbol(SymbolTable *, char *);
void printSymbolTable(SymbolTable *);
char *getSymbolTableEntry(SymbolTable *, int);

#endif
