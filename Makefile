LEXER_H = $(wildcard src/lexer/*.h)
SYMTAB_H = $(wildcard src/symbol_table/*.h)
PARSER_H = $(wildcard src/parser/*.h)

LEXER_SRC = $(wildcard src/lexer/*.cc)
SYMTAB_SRC = $(wildcard src/symbol_table/*.cc)
PARSER_SRC = $(wildcard src/parser/*.cc)

TLEXER_SRC = tests/lexer/test_lexer.cc
TLEXER = $(TLEXER_SRC:.cc=)

TPARSER_SRC = tests/parser/test_parser.cc
TPARSER = $(TPARSER_SRC:.cc=)

LEXER_OBJ = $(LEXER_SRC:.cc=.o)
SYMTAB_OBJ = $(SYMTAB_SRC:.cc=.o)
PARSER_OBJ = $(PARSER_SRC:.cc=.o)


$(SYMTAB_OBJ): $(SYMTAB_SRC) $(SYMTAB_H)

$(LEXER_OBJ): $(LEXER_SRC) $(LEXER_H) $(SYMTAB_H)

$(PARSER_OBJ): $(PARSER_SRC) $(LEXER_H) $(SYMTAB_H) $(PARSER_H)

$(TLEXER): $(SYMTAB_OBJ) $(LEXER_OBJ) $(TLEXER_SRC)

$(TPARSER): $(SYMTAB_OBJ) $(LEXER_OBJ) $(PARSER_OBJ) $(TPARSER_SRC)

.PHONY: tests
tests: $(TLEXER) $(TPARSER)
	@sh tests/run_tests.sh

.PHONY: clean
clean:
	-rm -f src/**/*.o  $(TLEXER) $(TPARSER)
