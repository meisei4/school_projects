Letter	[A-Za-z]
Digit   [0-9]

%{
  #include "IOMngr.h"
  #define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

    bool shouldLog = false;
    unsigned int tokStrt;

    #define RETURN(val,offset) \
      { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,(unsigned long)yyleng,yytext); \
        tokStrt = GetCurrentColumn() - yyleng + offset; \
        return(val); \
      }

    int IN_COMMENT = false;
    int  nest = 0;

    #include "YSemantics.h"
    #include "y.tab.h"

%}

%%
"if"                            { if (!IN_COMMENT) RETURN(IF_TOK, 0); }
"else"                          { if (!IN_COMMENT) RETURN(ELSE_TOK, 0); }
"while"                         { if (!IN_COMMENT) RETURN(WHILE_TOK, 0); }
"for"                           { if (!IN_COMMENT) RETURN(FOR_TOK, 0); }
"get"                           { if (!IN_COMMENT) RETURN(GET_TOK, 0); }
"put"                           { if (!IN_COMMENT) RETURN(PUT_TOK, 0); }
"decl"                          { if (!IN_COMMENT) RETURN(DECL_TOK, 0); }
"impl"                          { if (!IN_COMMENT) RETURN(IMPL_TOK, 0); }
"chr"                           { if (!IN_COMMENT) RETURN(CHR_TOK, 0); } 
"int"                           { if (!IN_COMMENT) RETURN(INT_TOK, 0); }
"loop"                          { if (!IN_COMMENT) RETURN(LOOP_TOK, 0); }
"break"                         { if (!IN_COMMENT) RETURN(BREAK_TOK, 0); } 
\'{Letter}\'                    { if (!IN_COMMENT) RETURN(CHRLIT_TOK, 0); }
\'"\\n"\'                       { if (!IN_COMMENT) RETURN(CHRLIT_TOK, 0); }
\'"<"\'                         { if (!IN_COMMENT) RETURN(CHRLIT_TOK, 0); }
\'">"\'                         { if (!IN_COMMENT) RETURN(CHRLIT_TOK, 0); }
\'"="\'                         { if (!IN_COMMENT) RETURN(CHRLIT_TOK, 0); } 
{Letter}({Letter}|{Digit})*	    { if (!IN_COMMENT) RETURN(IDENT_TOK,0); }
-?{Digit}+                      { if (!IN_COMMENT) RETURN(INTLIT_TOK, 0); }
\"(\\.|[^"\\])*\"               { if (!IN_COMMENT) RETURN(STRLIT_TOK, 0); }
"||"                            { if (!IN_COMMENT) RETURN(OR_TOK, 0); }
"&&"                            { if (!IN_COMMENT) RETURN(AND_TOK, 0); }
"//"                            { IN_COMMENT = true; }
"/*"                            { nest++; IN_COMMENT = true; }
"*/"                            { nest--; if(!nest) IN_COMMENT = false; }
"##"                            { if (!IN_COMMENT) RETURN(NOTEQUAL_TOK, 0); }
"=="                            { if (!IN_COMMENT) RETURN(EQUAL_TOK, 0); }
"<="                            { if (!IN_COMMENT) RETURN(LTE_TOK, 0); }
">="                            { if (!IN_COMMENT) RETURN(GTE_TOK, 0); }
\<                              { if (!IN_COMMENT) RETURN('<', 0); }
\>                              { if (!IN_COMMENT) RETURN('>', 0); }
\=                              { if (!IN_COMMENT) RETURN('=',1); }
\;                              { if (!IN_COMMENT) RETURN(';',1); }
\:                              { if (!IN_COMMENT) RETURN(':',1); }
\(				                { if (!IN_COMMENT) RETURN('(',1); }
\)				                { if (!IN_COMMENT) RETURN(')',1); }
\+				                { if (!IN_COMMENT) RETURN('+',1); }
\-				                { if (!IN_COMMENT) RETURN('-',1); }
\*				                { if (!IN_COMMENT) RETURN('*',1); }
\/				                { if (!IN_COMMENT) RETURN('/',1); }
\{							    { if (!IN_COMMENT) RETURN('{',1); }
\}                              { if (!IN_COMMENT) RETURN('}',1); }
\,                              { if (!IN_COMMENT) RETURN(',',1); }
[ \t]                           { }
[\n]                            { if(!nest) IN_COMMENT = false; }
.                               { }

%%
