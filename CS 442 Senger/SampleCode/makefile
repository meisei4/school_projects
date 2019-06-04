CFLAGS = -g -std=gnu11
YFLAGS = -d
LOADLIBES = -ll -ly

SampleAux.o:	SampleAux.c SampleAux.h
Sample.o:     Sample.c SampleAux.h

Sample:				Sample.o SampleAux.o

D2E.o: 				D2E.y
Scanner.o:		Scanner.l

D2E: 					D2E.o Scanner.o

test: Sample HighLight D2E
	./Sample
	./HighLight all < HighLight_data
	./D2E < D2E.in

clean:
	rm -rf *.o *.dSYM Sample HighLight D2E.c D2E sizes.c sizes
