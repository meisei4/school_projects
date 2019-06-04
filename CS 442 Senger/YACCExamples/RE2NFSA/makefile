YFLAGS = -d
CFLAGS = -g  -std=gnu11
CPPFLAGS = -g 
LOADLIBES = -ll -ly

grammar.o:			grammar.y NFSA.h
scanner.o:			scanner.l
NFSA.o:					NFSA.cpp NFSA.h

NFSA: 	NFSA.o grammar.o scanner.o
	c++ NFSA.o grammar.o scanner.o -ll -ly -o NFSA

clean:
		rm *.o NFSA
