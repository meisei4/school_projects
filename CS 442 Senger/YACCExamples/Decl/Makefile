YFLAGS = -d
CFLAGS = -g  -std=gnu11
LOADLIBES = -ll -ly

Decl.o: 	Decl.y
Scanner.o:	Scanner.l

Decl: 	Decl.o Scanner.o

clean:
		rm *.o Decl
