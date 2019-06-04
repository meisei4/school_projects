%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "IOMngr.h"
  #include "Scanner.h"
  #include "YSemantics.h"

void yyerror(char *s);
  %}

/* Union structure of data that can be attached to non-terminals   */
%union     {
// real-type union-field-name
  int Integer;
  char * Text;
  struct IdList * IdList;
  enum BaseTypes BaseType;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
  enum Op Op;
}

/* Type declaration for data attached to non-terminals. Allows     */
/* $# notation to be correctly type casted for function arguments. */
//    <union-field-name> non-terminal                        
%type <Text> Id
%type <IdList> DeclList
%type <IdList> DeclItem
%type <InstrSeq> DeclImpls
%type <BaseType> Type
%type <InstrSeq> FuncBody
%type <InstrSeq> FuncStmts
%type <InstrSeq> Stmt
%type <CondResult> Cond
%type <InstrSeq> IfStmt
%type <InstrSeq> WhileStmt
%type <InstrSeq> ForStmt
%type <InstrSeq> AssignStmt
%type <ExprResult> Expr
%type <ExprResult> Term
%type <ExprResult> Factor
%type <ExprResult> Get
%type <InstrSeq> Crement
%type <InstrSeq> Increment
%type <InstrSeq> Decrement
%type <InstrSeq> Put
%type <Op> AddOp
%type <Op> MultOp
%type <CondResult> Boolean

/* List of token name and corresponding numbers */
/* y.tab.h will be generated from these */
%token IDENT_TOK  	1
%token DECL_TOK   	2
%token IMPL_TOK   	3
%token INT_TOK      4
%token CHR_TOK      5
%token INTLIT_TOK   6 
%token CHRLIT_TOK   7
%token GET_TOK      8
%token PUT_TOK      9
%token IF_TOK       10
%token ELSE_TOK     11
%token NOTEQUAL_TOK 12
%token WHILE_TOK    13
%token EQUAL_TOK    14
%token STRLIT_TOK   15
%token BOOL_TOK     16
%token AND_TOK      17
%token OR_TOK       18
%token FOR_TOK      19
%token LTE_TOK      20
%token GTE_TOK      21
%token BREAK_TOK    22
%token LOOP_TOK     23
// can't go past 32 without conflicting with single char tokens
// could use larger token numbers

%%

Module        : DeclImpls                                       { Finish(); };
DeclImpls     : Decl DeclImpls                                  { };
DeclImpls     : Impl DeclImpls                                  { };
DeclImpls     :                                                 { };
Decl          : DECL_TOK DeclList ':' Type ';'                  { ProcDecls($2, $4); };
DeclList      : DeclItem ',' DeclList                           { $$ = AppendIdList($1, $3); };
DeclList      : DeclItem                                        { $$ = $1; };
DeclItem      : Id                                              { $$ = ProcName($1, PrimType); };
DeclItem      : Id FuncArgTypes                                 { $$ = ProcName($1, FuncType); }; 
Id            : IDENT_TOK                                       { $$ = strdup(yytext); };
Get           : GET_TOK '(' INT_TOK ')'                         { $$ = Get(); };
FuncArgTypes  : '(' ')'                                         {  };
Type          : INT_TOK                                         { $$ = IntBaseType; };
Type          : CHR_TOK                                         { $$ = ChrBaseType; };
Impl          : IMPL_TOK Id FuncArgNames FuncBody ';'           { ProcFunc($2, $4); };
FuncArgNames  : '(' ')'                                         {  };
FuncBody      : '{' FuncStmts '}'                               { $$ = $2; };
FuncStmts     : Stmt ';' FuncStmts                              { $$ = AppendSeq($1, $3); ResetAllTmpReg(); };
FuncStmts     :                                                 { $$ = GenInstr(NULL, NULL, NULL, NULL, NULL); };

Stmt          : IfStmt                                          { $$ = $1; };
Stmt          : ForStmt                                         { $$ = $1; };
Stmt          : WhileStmt                                       { $$ = $1; };
Stmt          : AssignStmt                                      { $$ = $1; };
Stmt          : Put                                             { $$ = $1; };
Stmt          : Crement                                         { $$ = $1; };
//ATOMIC INC/DEC
Crement       : Increment                                       {  };
Crement       : Decrement                                       {  };
Increment     : Id '+' '+'                                      { $$ = Crement($1, "add"); };
Decrement     : Id '-' '-'                                      { $$ = Crement($1, "sub"); };

//CONTROL 
IfStmt        : IF_TOK '(' Cond ')' FuncBody ELSE_TOK FuncBody  { $$ = IfBranch($3, $5, $7); };
IfStmt        : IF_TOK '(' Cond ')' FuncBody                    { $$ = IfBranch($3, $5, NULL); };
WhileStmt     : WHILE_TOK '(' Cond ')' FuncBody                 { $$ = While($3, $5); };
/*
BreakedLoop   : LOOP_TOK '{' 
                    FuncStmts 
                    IF_TOK '(' Cond ')' '{' 
                        FuncStmts BREAK_TOK ';'
                    '}'                                         { }; 
*/

Cond          : Boolean                                         { $$ = $1; };
Boolean       : Expr '<' Expr                                   { $$ = Compare($1, $3, "bge"); };
Boolean       : Expr '>' Expr                                   { $$ = Compare($1, $3, "ble"); };
Boolean       : Expr GTE_TOK Expr                               { $$ = Compare($1, $3, "bl"); };
Boolean       : Expr LTE_TOK Expr                               { $$ = Compare($1, $3, "bg"); };
Boolean       : Expr NOTEQUAL_TOK Expr                          { $$ = Compare($1, $3, "beq"); }
Boolean       : Expr EQUAL_TOK Expr                             { $$ = Compare($1, $3, "bne"); };

//FOR LOOP
ForStmt       : FOR_TOK '(' AssignStmt ';' Cond ';' Crement ';' ')' 
                FuncBody                                        { $$ = For($3, $5, $7, $10); };
Put           : PUT_TOK '(' Expr ')'                            { $$ = Put($3); };
AssignStmt    : Id '=' Expr                                     { $$ = Store($1, $3); };
Expr          : Expr AddOp Term                                 { $$ = Arithmetic($1, $3, $2, IntBaseType); };
Expr          : Term                                            { $$ = $1; };
Expr          : Get                                             { $$ = $1; };
Expr          : CHRLIT_TOK                                      { $$ = Load(strdup(yytext), ChrBaseType); };
Expr          : STRLIT_TOK                                      { $$ = ProcString(strdup(yytext)); };
Term          : Term MultOp Factor                              { $$ = Arithmetic($1, $3, $2, IntBaseType); };
Term          : Factor                                          { $$ = $1; };
Factor        : '(' Expr ')'                                    { $$ = $2; };
Factor        : '-' Factor                                      { $$ = Negate($2); };
Factor        : INTLIT_TOK                                      { $$ = Load(strdup(yytext), IntBaseType); };
Factor        : IDENT_TOK                                       { $$ = Load(strdup(yytext), IntBaseType); };
AddOp         : '+'                                             { $$ = Add; };
AddOp         : '-'                                             { $$ = Sub; };
MultOp        : '*'                                             { $$ = Mult;};
MultOp        : '/'                                             { $$ = Div; };


%%

void
yyerror( char *s)     {
  char msg[MAXLINE];
  sprintf(msg,"ERROR \"%s\" token: \"%s\"",s,yytext);
  PostMessage(GetCurrentColumn(),strlen(yytext),msg);
}
