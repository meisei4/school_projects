CFLAGS = -g -std=gnu11


test:
	./match AAGAGAAGAAAC < DNA-22
	./od -x -a <bin-data
	./od -o -g 2 -l 64 <bin-data
	./od -d -g 4 -l 64 <bin-data

clean:
	rm match od