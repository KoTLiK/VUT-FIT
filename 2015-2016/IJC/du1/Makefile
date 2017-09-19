#
# Makefile
# Riesenie IJC-DU1, priklad b), 8.3.2016
# Autor: Milan Augustín, xaugus09
# Prelozene: gcc 4.9
# 

CC=gcc
CFLAGS=-O2 -std=c99 -Wall -pedantic
LOADLIBES=-lm

all: primes primes-inline steg-decode

bit_array.h: error.o
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o $@
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o $@

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) primes.o eratosthenes.o error.o $(LOADLIBES) -o $@
primes.o: primes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -c primes.c -o $@
eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@

primes-inline: primes-inline.o eratosthenes-inline.o error.o
	$(CC) $(CFLAGS) primes-inline.o eratosthenes-inline.o error.o $(LOADLIBES) -o $@
primes-inline.o: primes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o $@
eratosthenes-inline.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o $@

steg-decode: steg-decode.o eratosthenes.o error.o ppm.o
	$(CC) $(CFLAGS) steg-decode.o eratosthenes.o error.o ppm.o -lm -o $@
steg-decode.o: steg-decode.c error.h ppm.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-decode.c -o $@

zip:
	zip xaugus09.zip *.c *.h Makefile

clean:
	rm -fv *.o

clean-all:
	rm -fv *.o primes primes-inline steg-decode