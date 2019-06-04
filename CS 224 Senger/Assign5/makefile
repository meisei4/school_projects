CFLAGS = -g -std=gnu11 -fno-common

intstk-r.o:	intstk-r.c intstk.h
intstk-l.o:	intstk-l.c intstk.h

stksort.o:	stksort.c intstk.h

stksort-r:	stksort.o intstk-r.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
stksort-l:	stksort.o intstk-l.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

expreval.o:	expreval.c intstk.h

expreval-r:	expreval.o intstk-r.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
expreval-l:	expreval.o intstk-l.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm *.o stksort-l stksort-r expreval-l expreval-r
