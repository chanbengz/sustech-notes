%{
    #define YYSTYPE char *
    #include "syntax.tab.h"
    #include "string.h"
%}

digitLetter [0-9a-fA-F]

%%
{digitLetter}+ { yylval = strdup(yytext); return X; }
\. { return DOT; }
: { return COLON; }
\n {}
. {return *yytext;}
%%
