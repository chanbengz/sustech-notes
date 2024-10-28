CC=gcc
FLEX=flex
BISON=bison
ip:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c -lfl -ly -o ip.out
clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
.PHONY: ip
