CFLAGS = -g -std=gnu11 -fno-common -Ihiredis
LDLIBS = hiredis/libhiredis.a


inter-httpd.o:	inter-httpd.c
inter-httpd:	inter-httpd.o


clean:	
	rm *.o inter-httpd