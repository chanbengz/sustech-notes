CC=gcc
FLEX=flex
BISON=bison


.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d syntax.y
jp: .lex .syntax
	$(CC) syntax.tab.c -lfl -ly -o jp.out
clean:
	@rm -f lex.yy.c syntax.tab.* *.out
