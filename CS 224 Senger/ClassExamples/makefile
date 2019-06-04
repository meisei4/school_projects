# set C compile flags for debug, gnu11 version and no common section
CFLAGS = -g -std=gnu11 -fno-common
LDLIBS = -lcurl

# target to clean directory of build products
# assumes everything that comes from a .c file name by removing the extension
# is an executable
executables := $(patsubst %.c,%,$(wildcard *.c))

.PHONY: clean

clean:
	rm -fr *.o *.dSYM jsmn $(executables)

#
# dependencies for multi file projects
#
multi-1.o:	multi-1.c multi-1.h
multi.o:	multi.c multi-1.h
multi: 		multi.o multi-1.o

typelist.o:	typelist.c typelist.h 
typedef.o:	typedef.c typelist.h
typedef:	typedef.o typelist.c

intstk.o:	intstk.c intstk.h
expreval.o:	expreval.c intstk.h
expreval:	expreval.o intstk.o
stksort.o:	stksort.c intstk.h
stksort:	stksort.o intstk.o

keylist.o:	keylist.c keylist.h
keyinsert.o:	keyinsert.c keylist.h
keyinsert:	keyinsert.o keylist.o

da.o:   	da.c da.h
da-test.o:  	da-test.c da.c
da-test:  	da-test.o da.o

bst-a.o:	bst-a.c da.h
bst-a:		bst-a.o da.o

tringbuf:	tringbuf.o ringbuf.o

ringbuf-a.o:	ringbuf-a.c ringbuf.h
tringbuf-a:	tringbuf.o ringbuf-a.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

hashtbl.o:	hashtbl.c hashtbl.h
thash:		thash.o hashtbl.o hashfn.o
netkv:		netkv.o hashtbl.o hashfn.o

jsmn:
	git clone https://github.com/zserge/jsmn

jsmn/jsmn.h:	jsmn 

jsmn/libjsmn.o:	jsmn/jsmn.c jsmn/jsmn.h
	$(CC) -c -DJSMN_PARENT_LINKS=1 $(CFLAGS) $< -o $@
 
jsondump.o:	jsondump.c jsmn/jsmn.h
jsondump:	jsondump.o jsmn/libjsmn.o

