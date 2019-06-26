/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENT_TOK = 1,
    DECL_TOK = 2,
    IMPL_TOK = 3,
    INT_TOK = 4,
    CHR_TOK = 5,
    INTLIT_TOK = 6,
    CHRLIT_TOK = 7,
    GET_TOK = 8,
    PUT_TOK = 9,
    IF_TOK = 10,
    ELSE_TOK = 11,
    NOTEQUAL_TOK = 12,
    WHILE_TOK = 13,
    EQUAL_TOK = 14,
    STRLIT_TOK = 15,
    BOOL_TOK = 16,
    AND_TOK = 17,
    OR_TOK = 18,
    FOR_TOK = 19,
    LTE_TOK = 20,
    GTE_TOK = 21,
    BREAK_TOK = 22,
    LOOP_TOK = 23
  };
#endif
/* Tokens.  */
#define IDENT_TOK 1
#define DECL_TOK 2
#define IMPL_TOK 3
#define INT_TOK 4
#define CHR_TOK 5
#define INTLIT_TOK 6
#define CHRLIT_TOK 7
#define GET_TOK 8
#define PUT_TOK 9
#define IF_TOK 10
#define ELSE_TOK 11
#define NOTEQUAL_TOK 12
#define WHILE_TOK 13
#define EQUAL_TOK 14
#define STRLIT_TOK 15
#define BOOL_TOK 16
#define AND_TOK 17
#define OR_TOK 18
#define FOR_TOK 19
#define LTE_TOK 20
#define GTE_TOK 21
#define BREAK_TOK 22
#define LOOP_TOK 23

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "YGrammar.y" /* yacc.c:1921  */

// real-type union-field-name
  int Integer;
  char * Text;
  struct IdList * IdList;
  enum BaseTypes BaseType;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
  enum Op Op;

#line 120 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
