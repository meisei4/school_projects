%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();      /* The next token function. */
extern char *yytext;     /* The matched token text.  */
extern int yyleng;       /* The token text length.   */

void yyerror(char *s);

#define YYSTYPE char *

%}


%token ID    1
%token INT   2
%token CHR   3


%%

Decl     : IdList Type ';'          { printf("Decl\n"); } ;
IdList   : ID MoreList              { printf("IdList\n"); } ;
//IdList   : Id MoreList              { printf("IdList - %s\n",$1); } ;
//Id       : ID                       { $$ = strdup(yytext); }
MoreList : ',' IdList               { printf("MoreList\n"); } ;
MoreList : ':'                      { printf("MoreList\n"); } ;
Type     : INT                      { printf("Type\n"); } ;
Type     : CHR                      { printf("Type\n"); } ;


%%


int
main(int argc, char *argv[]) {
  yyparse();
}

void
yyerror(char *s) {
  printf("err: \"%s\" yytext: \"%s\"\n",s,yytext);
}
