#
# Makefile
# Riesenie IJC-DU2, 11.4.2016
# Autor: Milan Augustin, xaugus09, VUT-FIT
# 

C=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic

CC=g++
CCFLAGS=-std=c++11 -pedantic -Wall

LIBFILES=hash_function.o htab_init.o htab_init2.o htab_lookup_add.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o


all: tail tail2 wordcount wordcount-dynamic


#####################################
############# Priklad 1 #############
# C99
error.o: error.c error.h
	$(C) $(CFLAGS) -c error.c -o $@

tail: tail.o error.o
	$(C) $(CFLAGS) tail.o error.o -o $@
tail.o: tail.c tail.h error.h
	$(C) $(CFLAGS) -c tail.c -o $@

# C++11
tail2: tail2.o
	$(CC) $(CCFLAGS) tail2.o -o $@
tail2.o: tail2.cc tail2.h
	$(CC) $(CCFLAGS) -c tail2.cc -o $@


#####################################
############# Priklad 2 #############
# C++11
#wordcount-: wordcount-.o
#	$(CC) $(CCFLAGS) wordcount-.o -o $@
#wordcount-.o: wordcount-.cc
#	$(CC) $(CCFLAGS) -c wordcount-.cc -o $@


# C99
io.o: io.c io.h
	$(C) $(CFLAGS) -c io.c -o $@
wordcount.o: wordcount.c htable.h io.h
	$(C) $(CFLAGS) -c wordcount.c -o $@

wordcount: wordcount.o io.o libhtable.a
	$(C) $(CFLAGS) -static io.o wordcount.o libhtable.a -o $@
wordcount-dynamic: wordcount.o io.o libhtable.so
	$(C) $(CFLAGS) wordcount.o io.o -L. libhtable.so -o $@


# Library STATIC
libhtable.a: $(LIBFILES)
	ar rcs libhtable.a $(LIBFILES)
# Library DYNAMIC
libhtable.so: $(LIBFILES)
	$(C) $(CFLAGS) -shared -fPIC $(LIBFILES) -o libhtable.so

# Library functions
hash_function.o: hash_function.c htable.h
	$(C) $(CFLAGS) -c -fPIC hash_function.c -o $@
htab_init.o: htab_init.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_init.c -o $@
htab_init2.o: htab_init2.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_init2.c -o $@
htab_lookup_add.o: htab_lookup_add.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_lookup_add.c -o $@
htab_foreach.o: htab_foreach.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_foreach.c -o $@
htab_remove.o: htab_remove.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_remove.c -o $@
htab_clear.o: htab_clear.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_clear.c -o $@
htab_free.o: htab_free.c htable.h
	$(C) $(CFLAGS) -c -fPIC htab_free.c -o $@


#####################################

.PHONY: zip clean

zip:
	zip xaugus09.zip *.c *.cc *.h Makefile

clean:
	rm -fv *.o tail tail2 wordcount- libhtable.a libhtable.so wordcount wordcount-dynamic