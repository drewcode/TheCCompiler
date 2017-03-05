#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"
#include "../../src/symbol_table/sym_tab.h"
#include "../../src/parser/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

using std::vector;

int main(int argc, char *argv[]) {
	//char *source = "int main() { printf(12.5); return 0; }";
	if(argc != 2) {
		printf("Usage: test filename\n");
		return 1;
	}

	char *source_path = argv[1];
	FILE *source_handle = fopen(source_path, "r");
	if(source_handle == NULL) {
		printf("Failed to open file: %s\n", source_path);
		return 1;
	}

	fseek(source_handle, 0, SEEK_END);
	long source_size = ftell(source_handle);
	fseek(source_handle, 0, SEEK_SET);

	char *source = (char *) malloc(source_size + 1);
	fread(source, source_size, 1, source_handle);
	source[source_size] = '\0';
	fclose(source_handle);

	SymbolTable table;
	vector<Token *> token_list = tokenize(source, &table);

	free(source);
	runParser(&token_list, &table);

	return 0;
}
