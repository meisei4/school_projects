/* RDGrammar.h
Contains the grammar parsing routines for the grammar

                                                    Select Sets?
<Prog>      :== Ident { <StmtSeq> }                 Ident
<StmtSeq>   :== <Stmt> ; <StmtSeq>                  INT CHR Ident
<StmtSeq>   :==                                     }
<Stmt>      :== <Decl>                              INT CHR
<Stmt>      :== <Assign>                            Ident
<Decl>      :== <Type> <IDLst>                      INT CHR
<Type>      :== INT                                 INT
<Type>      :== CHR                                 CHR
<IDLst>     :== Ident <MLst>                        Ident
<MLst>      :== , <IDLst>                           ,
<MLst>      :==                                     ;
<Assign>    :== Ident := <Expr>                     Ident
<Expr>      :== <Term> <MExpr>                      ( - INTLIT FLOATLIT Ident
<MExpr>     :== <AddOp> <Term> <MExpr>              - + 
<MExpr>     :==                                     ; )
<Term>      :== <Factor> <MTerm>                    ( - INTLIT FLOATLIT Ident
<MTerm>     :== <MultOp> <Factor> <MTerm>           * /
<MTerm>     :==                                     - + ; )
<Factor>    :== ( <Expr> )                          (
<Factor>    :== - <Factor>                          -
<Factor>    :== <IntLit>                            INTLIT  
<Factor>    :== <FloatLit>                          FLOATLIT
<Factor>    :== <Ident>                             Ident
<AddOp>     :== -                                   -
<AddOp>     :== +                                   +
<MultOp>    :== *                                   *
<MultOp>    :== /                                   /
*/

#include "RDSupport.h"

/* Nonterminal Functions */
bool Prog();
bool StmtSeq();
bool Stmt();
bool Decl();
bool Type();
bool IDLst();
bool MLst();
bool Assign();
bool Expr();
bool MExpr();
bool Term();
bool MTerm();
bool Factor();
bool AddOp();
bool MultOp();
