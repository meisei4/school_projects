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
jsmn:
	git clone https://github.com/zserge/jsmn

jsmn/jsmn.h:	jsmn 

jsmn/libjsmn.o:	jsmn/jsmn.c jsmn/jsmn.h
	$(CC) -c -DJSMN_PARENT_LINKS=1 $(CFLAGS) $< -o $@
 
jsondump.o:	jsondump.c jsmn/jsmn.h
jsondump:	jsondump.o jsmn/libjsmn.o

jsonfac.o:	jsonfac.c jsmnutil.h jsmn/jsmn.h
jsonfac:	jsonfac.o jsmnutil.o jsmn/libjsmn.o
