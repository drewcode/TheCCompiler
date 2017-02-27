LEXER_H = $(wildcard src/lexer/*.h)
SYMTAB_H = $(wildcard src/symbol_table/*.h)

LEXER_SRC = $(wildcard src/lexer/*.cc)
SYMTAB_SRC = $(wildcard src/symbol_table/*.cc)

TLEXER_SRC = tests/lexer/test_lexer.cc
TLEXER = $(TLEXER_SRC:.cc=)

LEXER_OBJ = $(LEXER_SRC:.cc=.o)
SYMTAB_OBJ = $(SYMTAB_SRC:.cc=.o)


$(SYMTAB_OBJ): $(SYMTAB_SRC) $(SYMTAB_H)

$(LEXER_OBJ): $(LEXER_SRC) $(LEXER_H) $(SYMTAB_H)

$(TLEXER): $(SYMTAB_OBJ) $(LEXER_OBJ) $(TLEXER_SRC)

.PHONY: tests
tests: $(TLEXER)
	@sh tests/run_tests.sh

.PHONY: clean
clean:
	-rm -f src/**/*.o  $(TLEXER)
