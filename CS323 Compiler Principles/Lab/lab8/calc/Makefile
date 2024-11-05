CC=gcc
FLEX=flex
BISON=bison
calc:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c -lfl -ly -o calc.out
clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out syntax.output
.PHONY: calc
