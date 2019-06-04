CFLAGS = -g -std=gnu11

ec.o:		ec.c ec.h
ec-test.o:	ec-test.c ec.h
ec-test:	ec-test.o ec.o

shotgun.o:	shotgun.c ec.h
shotgun:	shotgun.o ec.o

concomp.o:  	concomp.c ec.h -lm
concomp:  	concomp.o ec.o -lm

clean:
	rm *.o ec-test concomp shotgun
