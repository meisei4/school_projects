# CS 224 - Code for Examples Done In Class

hw.c          - hello world example

basetypes.c   - display a table of base type sizes and min/max values

gcd-1.c       - compute greatest common denomiator of two variables

fac-1.c       - compute factorial of numbers up to 10

ptest-1.c     - test numbers up to 100 for primeness

flag.c        - produce raw image data for a US flag with a single star,
                50 stars is left as an exercise for the reader, raw data can
                be converted to png with graphicsmagick convert command

decho.c       - echo input to output but display non-printing in decimal

copycons.c    - copy input to output removing all vowels

indent.c      - echo input to output replacing initial dash char on line by tab

wavg.c        - count words (consecutive alpha sequences), compute avg word length

sumdigits.c   - read input looking for numbers, sum all numbers found

wlh.c			 - construct histogram of word lengths

scramble.c    - read input scrambling the interior letters of every word using the Fischer/Yates/Durstenfeld/Knuth algorithm

revwords.c    - copy input to output reversing the characters in each word

merge.c 		 - merge ordered lists

strlit.c 		 - various experiments with string literals

staramp.c 	 - various experiments with * and &

pbr.c         - simple example of passing arrays and integers by reference

cf-exp1.c     - experiment with pass by reference

parg.c        - display the commane line arguments used in invoking the program

pal.c         - test if command line argument is a palindrome

gcd.c         - compute gcd of command line arguments or stdin

getopt.c      - demonstrate use of the getop() function for processing command line options and arguments

env.c         - demonstrate getenv()

main3.c       - simple illustration of 3 argument main function

bitop.c       - demonstrate use of bit operators

bitflags.c    - sample use of bit operations for flags

multi.c
multi-1.h/c   - demonstrate multiple file compilation, linking and extern variables

strfunc.c     - implementation and demonstration of common string.h functions

errno.c       - detect errors from setenv() call

vla.c         - demonstrate variable length arrays

tmalloc.c     - malloc memory and print memory contents

growbuf.c     - demonstrate use of realloc to create extensible buffer

msizes.c      - demonstrate malloc size increments

funcarg.c     - demonstrate declaration, storage and passing function addresses

qsort.c       - demonstrate passing functions using the standard library qsort function.

keylist-x.c   - various version (0-5) of a insertion list

keylist.h/c   - opaque implementation of list type

keyinsert.c   - driver for testing keylist.h/c

typelist.h/c  - implementation of a simple struct based linked list with addList() and printList()

typedef.c     - driver for typelist implementation

intstk.h/c    - implementation of stack of integers using realloc()

stksort.c     - sort input using stacks

expreval.c    - evaluation of expressions using stacks

multiarray.c  - demonstration of multi-dimensional array and array of arrays

chopstr.c     - chop argument string into whitespace separated substrings, display substrings

bitfield.c	 - demonstrate the use of struct bitfields

da.h/c     	 - opaque implementation of dynamic arrays

da-test.c     - simple test driver for da.h

bst-m.c       - binary search tree example using malloc'ed nodes

bst-a.c       - binary search tree example that stores tree in a dynamic array

tcurl.c       - use libcurl to request and capture a URL

curlquote.c   - use libcurl to get a random famous quote

netcat.c      - example program using network sockets

texec.c       - example use of execve()

tfork.c       - example use of fork()

tsignal.c     - example of signals (SIGSEGV, SIGALRM, SIGINT)

tforksig.c    - example of forking child processes and controlling with signals

httpd.c       - simple web server

ttime.c       - example use of time(), localtime() and strftime()

netcat2.c     - allows bidirectional I/O between communicating programs using select() and file descriptor sets

chatsvr.c     - simple message exchange server

tmmap.c       - maps file and calculates checksum

cksum.c       - uses getChar() and calculates checksum

tsmem.c       - demonstrates shared memory between processes to exchange messages

ringbuf.h/c   - ring buffer implementation

tringbuf.c    - test driver for ring buffer

varg.c        - simple variable argument list example

hashtbl.h/c   - key/value store implemented as hash table

hashfn.h/c    - simple char sum hash and FNV hash implementations

thash.c       - simple test of key/value store implementation

netkv.c       - implementation of network key/value server using hash table

jsondump.c    - example of using JSMN library to parse JSON string and dump token array

tthread.c     - simple example of pthreads

netcat3.c     - pthread implementation of netcat

tseek.c       - example of using seek() file I/O function
