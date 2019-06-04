Digit [0-1]

%{
#include "y.tab.h"
%}

%%
{Digit}  	  {  return LETTER_TOK; }
\"\"        {  return EMPTY_STR_TOK; }
\{\}        {  return EMPTY_SET_TOK; }
\+       		{  return '+'; }
\^       		{  return '^'; }
\*       		{  return '*'; }
\(       		{  return '('; }
\)       		{  return ')'; }

[ \n\t]		{ }
.					{ }
%%
