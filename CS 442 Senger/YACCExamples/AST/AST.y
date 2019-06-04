%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();     /* The next token function. */
extern char *yytext;    /* The matched token text.  */
extern int yyleng;      /* The token text length.   */

void yyerror(char *s);

#define YYSTYPE void *

#include "PrintTree.h"

#define OUT_ASCII 0
#define OUT_HTML 1
#define OUT_ASM 2
#define OUT_ASMREG 3
int OutKind = OUT_ASMREG;

%}


%token BEGIN   1  END     2
%token Ident   3  IntLit  4
%token EQUAL   5
%token PLUS    6  MINUS   7  TIMES      8  DIVIDES  9
%token LPAREN 10  RPAREN 11  SEMICOLON 12

%%

Prog    : BEGIN StmtSeq END       { switch (OutKind) {
                                      case OUT_ASCII:
                                        PrintAST($2,0);
                                        break;
                                      case OUT_HTML:
                                        PrintSVGAST($2);
                                        break;
                                      case OUT_ASM:
                                        PrintAsm($2);
                                        break;
                                      case OUT_ASMREG:
                                        PrintAsmReg($2);
                                        break;
                                    }
                                  } ;
StmtSeq : Stmt StmtSeq            { struct node * new = malloc(sizeof(struct node));
                                    new->type = BLOCK;
                                    new->block.stmt = $1;
                                    new->block.block = $2;
                                    $$ = new; } ;
StmtSeq :                         { $$ = NULL; } ;
Stmt    : Id EQUAL Expr SEMICOLON { struct node * new = malloc(sizeof(struct node));
                                    new->type = ASSIGN;
                                    new->assign.lhs = $1;
                                    new->assign.expr = $3;
                                    $$ = new;} ;
Expr    : Expr PLUS Term          { struct node * new = malloc(sizeof(struct node));
                                    new->type = BIOP;
                                    new->biop.op = "+";
                                    new->biop.oprnd1 = $1;
                                    new->biop.oprnd2 = $3;
                                    $$ = new; } ;
Expr    : Expr MINUS Term         { struct node * new = malloc(sizeof(struct node));
                                    new->type = BIOP;
                                    new->biop.op = "-";
                                    new->biop.oprnd1 = $1;
                                    new->biop.oprnd2 = $3;
                                    $$ = new; } ;
Expr    : Term                    { $$ = $1; } ;
Term    : Term TIMES Factor       { struct node * new = malloc(sizeof(struct node));
                                    new->type = BIOP;
                                    new->biop.op = "*";
                                    new->biop.oprnd1 = $1;
                                    new->biop.oprnd2 = $3;
                                    $$ = new; } ;
Term    : Term DIVIDES Factor     { struct node * new = malloc(sizeof(struct node));
                                    new->type = BIOP;
                                    new->biop.op = "/";
                                    new->biop.oprnd1 = $1;
                                    new->biop.oprnd2 = $3;
                                    $$ = new;
                                    } ;
Term    : Factor                  { $$ = $1; } ;
Factor  : LPAREN Expr RPAREN      { $$ = $2; } ;
Factor  : MINUS Factor            { struct node * new = malloc(sizeof(struct node));
                                    new->type = UNOP;
                                    new->unop.op = "-";
                                    new->unop.oprnd = $2;
                                    $$ = new; } ;
Factor  : IntLit                  { struct node * new = malloc(sizeof(struct node));
                                    new->type = LITERAL;
                                    new->literal.literal = strdup(yytext);
                                    $$ = new; } ;
Factor  : Id                      { $$ = $1; } ;
Id      : Ident                   { struct node * new = malloc(sizeof(struct node));
                                    new->type = IDENT;
                                    new->ident.ident = strdup(yytext);
                                    $$ = new; } ;

%%

int
main(int argc, char *argv[])
{
 yyparse();
}


void
yyerror(char *s)
{
  printf("err: \"%s\" yytext: \"%s\"\n",s,yytext);
}
