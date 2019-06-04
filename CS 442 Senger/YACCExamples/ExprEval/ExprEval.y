%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();      /* The next token function. */
extern char *yytext;     /* The matched token text.  */
extern int yyleng;       /* The token text length.   */

void yyerror(char *s);

#define YYSTYPE long     /* 64 bit so can hold pointer and int */

int vars[26];

%}


%token BEGIN   1   END      2
%token Ident   3   IntLit   4
%token EQUAL   5
%token PLUS    6   MINUS    7  TIMES      8   DIVIDES   9
%token LPAREN 10   RPAREN  11  SEMICOLON 12

%%

Prog    :  BEGIN StmtSeq END        { printf("Prog\n"); } ;
StmtSeq :  Stmt StmtSeq             { } ;
StmtSeq :                           { } ;
Stmt    :  LHS EQUAL Expr SEMICOLON { printf("assign: %c %ld\n",(char)$1,$3);
                                      vars[$1-'a'] = $3; } ;
LHS     :  Ident                    { printf("id: %s\n",yytext); $$ = yytext[0]; } ;
Expr    :  Expr PLUS Term           { $$ = $1 + $3; } ;
Expr    :  Expr MINUS Term          { $$ = $1 - $3; } ;
Expr    :  Term                     { $$ = $1; } ;
Term    :  Term TIMES Factor        { $$ = $1 * $3; } ;
Term    :  Term DIVIDES Factor      { $$ = $1 / $3; } ;
Term    :  Factor                   { $$ = $1; } ;
Factor  :  LPAREN Expr RPAREN       { $$ = $2; } ;
Factor  :  MINUS Factor             { $$ = - $2; } ;
Factor  :  IntLit                   { printf("lit: %s\n",yytext); $$ = atoi(yytext); } ;
Factor  :  Ident                    { $$ = vars[yytext[0]-'a']; }
%%

void
init() {
  for (int i = 0; i < 26; i++) vars[i] = 0;
}

int
main(int argc, char *argv[]) {
  init();
  yyparse();
}

void
yyerror(char *s) {
  printf("err: \"%s\" yytext: \"%s\"\n",s,yytext);
}
