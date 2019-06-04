/* Author:      Ian Neville
   Created:     10/24.18
   Resources:   Nic Welch, Steven Senger
*/
#include <stdbool.h>
#include <stdio.h>
#include "IOMngr.h"
#include "RDGrammar.h"
#include "RDTokens.h"

bool Prog()
//<Prog>      :== Ident { <StmtSeq> }                 Ident
{
    ENTER;
    if (!Match(IDENT_TOK)) return LEAVE_FAIL;
    if (!Match(LBRACE_TOK)) return LEAVE_FAIL;
    if (!StmtSeq()) return LEAVE_FAIL;
    if (!Match(RBRACE_TOK)) return LEAVE_FAIL;

    return LEAVE_SUCC;
}

bool StmtSeq()
//<StmtSeq>   :== <Stmt> ; <StmtSeq>                  INT CHR Ident
//<StmtSeq>   :==                                     }
{
    ENTER;
    switch (CurrentToken()) {
        case INT_TOK:
        case CHR_TOK:
        case IDENT_TOK:
            if (!Stmt()) return LEAVE_FAIL;
            if (!Match(SEMI_TOK)) return LEAVE_FAIL;
            if (!StmtSeq()) return LEAVE_FAIL;
            break;
        case RBRACE_TOK:
            break;
        default:
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool Stmt()
//<Stmt>      :== <Decl>                              INT CHR
//<Stmt>      :== <Assign>                            Ident
{ 
    ENTER;
    switch (CurrentToken()) {
        case INT_TOK:
        case CHR_TOK:
            if (!Decl()) return LEAVE_FAIL; 
            break;
        case IDENT_TOK:
            if (!Assign()) return LEAVE_FAIL;
            break;
        default: 
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool Decl()
//<Decl>      :== <Type> <IDLst>                      INT CHR        
{
    ENTER;
    if (!Type()) return LEAVE_FAIL;
    if (!IDLst()) return LEAVE_FAIL;
    return LEAVE_SUCC;
}

bool Type()
//<Type>      :== INT                                 INT
//<Type>      :== CHR                                 CHR
{
    ENTER;
    switch(CurrentToken()){
        case INT_TOK:
            Match(INT_TOK);
            break;
        case CHR_TOK:
            Match(CHR_TOK);
            break;
        default: 
            ParseError("No such switch option");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool IDLst()
//<IDLst>     :== Ident <MLst>                        Ident
{
    ENTER;
    if (!Match(IDENT_TOK)) return LEAVE_FAIL;
    if (!MLst()) return LEAVE_FAIL;
    return LEAVE_SUCC;
}

bool MLst()
//<MLst>      :== , <IDLst>                           ,
//<MLst>      :==                                     ;
{
    ENTER;
    switch (CurrentToken()) {
        case COMMA_TOK:
            Match(COMMA_TOK);
            if (!IDLst()) return LEAVE_FAIL;
            break;
        case SEMI_TOK:
            break;
        default:
            ParseError("No option in switch");
            return LEAVE_FAIL;  
            break;
    }
    return LEAVE_SUCC;
}

bool Assign()
//<Assign>    :== Ident := <Expr>                     Ident
{
    ENTER;
    if (!Match(IDENT_TOK)) return LEAVE_FAIL;
    if (!Match(ASSIGN_TOK)) return LEAVE_FAIL;
    if (!Expr()) return LEAVE_FAIL;
    return LEAVE_SUCC;
}

bool Expr()
//<Expr>      :== <Term> <MExpr>                      ( - INTLIT FLOATLIT Ident
{
    ENTER;
    if (!Term()) return LEAVE_FAIL;
    if (!MExpr()) return LEAVE_FAIL;
    return LEAVE_SUCC;
}

bool MExpr()
//<MExpr>     :== <AddOp> <Term> <MExpr>              - + 
//<MExpr>     :==                                     ; )
{
    ENTER;
    switch (CurrentToken()) {
        case MINUS_TOK:
        case PLUS_TOK:
            if (!AddOp()) return LEAVE_FAIL;
            if (!Term()) return LEAVE_FAIL;
            if (!MExpr()) return LEAVE_FAIL;
            break;
        case SEMI_TOK:
        case RPAREN_TOK:
            break;
        default: 
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool Term()
//<Term>      :== <Factor> <MTerm>                    ( - INTLIT FLOATLIT Ident
{
    ENTER;
    if (!Factor()) return LEAVE_FAIL;
    if (!MTerm()) return LEAVE_FAIL;
    return LEAVE_SUCC;
}

bool MTerm()
//<MTerm>     :== <MultOp> <Factor> <MTerm>           * /
//<MTerm>     :==                                     - + ; )
{
    ENTER;
    switch (CurrentToken()) {
        case TIMES_TOK:
        case DIV_TOK:
            if (!MultOp()) return LEAVE_FAIL;
            if (!Factor()) return LEAVE_FAIL;
            if (!MTerm()) return LEAVE_FAIL;
            break;
        case MINUS_TOK:
        case PLUS_TOK:
        case RPAREN_TOK:
        case SEMI_TOK:
            break;
        default:
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool Factor()
//<Factor>    :== ( <Expr> )                          (
//<Factor>    :== - <Factor>                          -
//<Factor>    :== <IntLit>                            INTLIT  
//<Factor>    :== <FloatLit>                          FLOATLIT
//<Factor>    :== <Ident>                             Ident
{
    ENTER;
    switch (CurrentToken()) {
        case LPAREN_TOK:
            Match(LPAREN_TOK);
            if (!Expr()) return LEAVE_FAIL;
            if (!Match(RPAREN_TOK)) return LEAVE_FAIL;
            break;
        case MINUS_TOK:
            Match(MINUS_TOK);
            if (!Factor()) return LEAVE_FAIL;
            break;
        case INTLIT_TOK:
            Match(INTLIT_TOK);
            break;
        case FLOATLIT_TOK:
            Match(FLOATLIT_TOK);
            break;
        case IDENT_TOK:
            Match(IDENT_TOK);
            break;
        default: 
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool AddOp()
//<AddOp>     :== -                                   -
//<AddOp>     :== +                                   +
{
    ENTER;
    switch (CurrentToken()) {
        case MINUS_TOK:
            Match(MINUS_TOK);
            break;
        case PLUS_TOK:
            Match(PLUS_TOK);
            break;
        default:
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}

bool MultOp()
//<MultOp>    :== *                                   *
//<MultOp>    :== /                                   /
{
    ENTER;
    switch (CurrentToken()) {
        case TIMES_TOK:
            Match(TIMES_TOK);
            break;
        case DIV_TOK:
            Match(DIV_TOK);
            break;
        default:
            ParseError("No option in switch");
            return LEAVE_FAIL;
            break;
    }
    return LEAVE_SUCC;
}
