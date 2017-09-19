#
# Makefile
# Riesenie IOS - projekt 2
# Autor: Milan Augustín, xaugus09
# Prelozene: gcc 4.8
# 

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LOADLIBES=-pthread
OBJFILES=rollercoaster.o arguments.o resources.o processes.o

all: proj2


proj2: $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) $(LOADLIBES) -o $@
rollercoaster.o: rollercoaster.c rollercoaster.h
	$(CC) $(CFLAGS) -c rollercoaster.c -o $@

arguments.o: arguments.c rollercoaster.h
	$(CC) $(CFLAGS) -c arguments.c -o $@
resources.o: resources.c rollercoaster.h
	$(CC) $(CFLAGS) -c resources.c -o $@
processes.o: processes.c rollercoaster.h
	$(CC) $(CFLAGS) -c processes.c -o $@


.PHONY: zip clean clean-all

zip:
	zip xaugus09.zip *.c *.h Makefile

clean:
	rm -fv *.o

clean-all:
	rm -fv *.o proj2 proj2.out