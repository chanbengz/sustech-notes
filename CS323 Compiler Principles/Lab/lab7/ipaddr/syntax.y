%{
    #define YYSTYPE char *
    #include "lex.yy.c"
    int yyerror(char* s);
%}

%token X
%token DOT
%token COLON

%%

IP: IPv4Exp { printf("IPv4\n"); }
    | IPv6Exp { printf("IPv6\n"); }
    ;
IPv4Exp: IPv4 DOT IPv4 DOT IPv4 DOT IPv4;
IPv4: X { 
    if (strlen($1) > 3 || (strlen($1) > 1 && $1[0] == '0')) {
        yyerror("");
        return 0;
    }
    if (atoi($1) > 255 || atoi($1) < 0) {
        yyerror("");
        return 0;
    } }
    ;
IPv6Exp: IPv6 COLON IPv6 COLON IPv6 COLON IPv6 COLON IPv6 COLON IPv6 COLON IPv6 COLON IPv6;
IPv6: X {
    if (strlen($1) > 4 || strlen($1) < 1) {
        yyerror("");
        return 0;
    } }
    ;

%%

int yyerror(char* s) {
    fprintf(stderr, "%s\n", "Invalid");
    return 1;
}
int main() {
    yyparse();
}
