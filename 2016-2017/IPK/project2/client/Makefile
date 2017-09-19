CC=g++
CCFLAGS=-std=c++11 -O2 -pedantic -Wall

all: ipk-client


ipk-client: main.o protocol.o client.o functions.o
	$(CC) $(CCFLAGS) main.o client.o protocol.o functions.o -o $@ -lcrypto
main.o: main.cpp Client.hpp header.hpp
	$(CC) $(CCFLAGS) -c main.cpp -o $@ -lssl -lcrypto

protocol.o: Protocol.cpp Protocol.hpp header.hpp
	$(CC) $(CCFLAGS) -c Protocol.cpp -o $@
client.o: Client.cpp Client.hpp Protocol.hpp header.hpp
	$(CC) $(CCFLAGS) -c Client.cpp -o $@
functions.o: functions.cpp header.hpp
	$(CC) $(CCFLAGS) -c functions.cpp -o $@

clean:
	rm -fv *.o ipk-client