CC=g++
CCFLAGS=-std=c++11 -pedantic -Wall

all: server


server: main.o protocol.o server.o functions.o
	$(CC) $(CCFLAGS) main.o server.o protocol.o functions.o -o $@
main.o: main.cpp Server.hpp header.hpp
	$(CC) $(CCFLAGS) -c main.cpp -o $@

protocol.o: Protocol.cpp Protocol.hpp header.hpp
	$(CC) $(CCFLAGS) -c Protocol.cpp -o $@
server.o: Server.cpp Server.hpp Protocol.hpp header.hpp
	$(CC) $(CCFLAGS) -c Server.cpp -o $@
functions.o: functions.cpp header.hpp
	$(CC) $(CCFLAGS) -c functions.cpp -o $@

clean:
	rm -fv *.o server