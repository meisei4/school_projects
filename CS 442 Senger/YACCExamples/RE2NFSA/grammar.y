%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NFSA.h"

extern int yylex();      /* The next token function. */
extern char *yytext;     /* The matched token text.  */
extern int yyleng;       /* The token text length.   */

void yyerror(char *s);

#define YYSTYPE struct NFSA *

%}

%token LETTER_TOK      1
%token EMPTY_STR_TOK   2
%token EMPTY_SET_TOK   3

%%
S     : RE                   { NFSA_Display($1); }
RE    : RE '+' RET           { $$ = NFSA_ForUnion($1,$3); };
RE    : RET                  { $$ = $1; };
RET   : RET '^' REF          { $$ = NFSA_ForConcat($1,$3); };
RET   : REF                  { $$ = $1; };
REF   : REF '*'              { $$ = NFSA_ForStar($1); };
REF   : '(' RE ')'           { $$ = $2; };
REF   : LETTER_TOK           { $$ = NFSA_Letter(yytext[0]-'0'); };
REF   : EMPTY_STR_TOK        { $$ = NFSA_EmptyStr(); };
REF   : EMPTY_SET_TOK        { $$ = NFSA_EmptySet(); };

%%

int
main(int argc, char *argv[]) {
  yyparse();
}

void
yyerror(char *s) {
  printf("err: \"%s\" yytext: \"%s\"\n",s,yytext);
}
