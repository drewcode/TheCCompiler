#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sym_tab.h"

int insertSymbol(SymbolTable *table, char *symbol) {
	int i;
	for(i = 0; i < table->size(); i++) {
		if(strcmp(symbol, table->operator[](i)->name) == 0) {
			// name already present in symbol table
			return i;
		}
	}

	// symbol not there
	SymbolTableEntry *new_entry = (SymbolTableEntry *) malloc(sizeof(SymbolTableEntry));
	new_entry->name = strndup(symbol, strlen(symbol));
	table->push_back(new_entry);
	return i;
}

void printSymbolTable(SymbolTable *table) {
	printf("Symbol Table\n");
	for(int i = 0; i < table->size(); i++) {
		printf("%d %s\n", i, table->operator[](i)->name);
	}
}
