CC=g++
CCFLAGS= -std=c++11 -O2 -pedantic -Wall

all: isabot


isabot: isabot.o functions.o config.o controller.o client.o
	$(CC) $(CCFLAGS) isabot.o functions.o config.o controller.o client.o -o $@

isabot.o: src/isabot.cpp src/common/common.h src/config/Config.h src/common/functions.h src/client/ClientProtocol.h src/protocol/Controller.h src/client/Client.h
	$(CC) $(CCFLAGS) -c src/isabot.cpp -o $@

functions.o: src/common/functions.cpp src/common/common.h src/common/functions.h src/config/Config.h
	$(CC) $(CCFLAGS) -c src/common/functions.cpp -o $@

config.o: src/config/Config.cpp src/common/common.h src/config/Config.h
	$(CC) $(CCFLAGS) -c src/config/Config.cpp -o $@

client.o: src/client/Client.cpp src/client/Client.h src/common/common.h src/protocol/Controller.h src/client/ClientProtocol.h
	$(CC) $(CCFLAGS) -c src/client/Client.cpp -o $@

controller.o: src/protocol/Controller.cpp src/protocol/Controller.h src/common/common.h src/client/ClientProtocol.h src/client/Client.h
	$(CC) $(CCFLAGS) -c src/protocol/Controller.cpp -o $@


clean:
	rm -fv *.o isabot

tar:
	tar -cvf xaugus09.tar src Makefile README manual.pdf
