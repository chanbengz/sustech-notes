%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}

%token LC RC LB RB COLON COMMA EMPTY
%token STRING NUMBER
%token TRUE FALSE VNULL
%left error
%left RB
%%

Json:
      Value
    ;
Value:
      Object 
    | Array
    | STRING
    | NUMBER
    | NUMBER NUMBER error { puts("Numbers cannot have leading zeroes, recovered"); }
    | TRUE
    | FALSE
    | VNULL
    | Array RB error { puts("Extra close, recovered"); }
    ;
Object:
      LC RC
    | LC Members RC
    | LC Members RC Value error { puts("misplaced quoted value, recovered"); }
    | LC Members COMMA error { puts("Comma instead of closing brace, recovered"); }
    ;
Members:
      Member 
    | Member COMMA Members 
    | Member COMMA error { puts("Extra comma, recovered"); }
    ;
Member:
      STRING COLON Value 
    | STRING Value error { puts("Missing colon, recovered"); }
    | STRING COLON COLON Value error { puts("Double colon, recovered"); }
    | STRING COMMA Value error { puts("Comma instead of colon, recovered"); }
    ;
Array:
      LB RB
    | LB Values RB
    | LB Values RC error { puts("mismatch, recovered"); }
    | LB Values error { puts("Unclosed array, recovered"); }
    | LB COMMA Values RB error { puts("missing value, recovered"); }
    | LB Values RB COMMA error { puts("Comma after the close, recovered"); }
    ;
Values:
      Value
    | Value COMMA Values
    | Value COMMA error { puts("extra comma, recovered"); }
    | Value COMMA COMMA error { puts("Double extra comma, recovered"); }
    | Value COLON Values error { puts("Colon instead of comma, recovered"); }
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}
