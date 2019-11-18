CFLAGS=-g -Wall -fopenmp

histogram: histogram.c

run: histogram
	./histogram 4
