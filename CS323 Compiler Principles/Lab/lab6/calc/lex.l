%{
    #include "syntax.tab.h"
    #include "stdlib.h"
%}
%%
[0-9]+ { yylval = atoi(yytext); return INT; }
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"(" { return LP; }
")" { return RP; }
[ \n\r\t] {}
. { fprintf(stderr, "unknown symbol: %s\n", yytext); exit(1); }
