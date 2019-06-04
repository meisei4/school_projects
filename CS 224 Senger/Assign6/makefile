CFLAGS = -g -std=gnu11 -fno-common
LDLIBS = -lcurl

ircbot.o:	ircbot.c
ircbot:		ircbot.o libircclient/src/libircclient.a

clean:
		rm -fr *.o *.dSYM hw
