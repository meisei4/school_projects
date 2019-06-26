Compilers course    
Non-Project:  
General reference code  

Project: 
Compiler and other related tools. Check Makefile for test information.   
Requires bison/yacc, flex/lex, and some version of "spim" MIPS simulator software.
One must change Makefile accordingly, (arch linux: "qtspim" and "LOADLIBES = -lfl" instead of "-ll" for lex lib)

*.src files are source code for "C-Like" language


COMPREHENSIVE TEST:
```
yacc -d YGrammar.y  
make ytest
```

Project/Semantics2: Part two of the compiler project
