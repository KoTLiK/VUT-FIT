CC=g++
CCFLAGS=-std=c++11 -O2 -pedantic -Wall

all: ftrestd ftrest


ftrestd: serve.o CoreException.o Message.o ARGS.o Server.o
	$(CC) $(CCFLAGS) serve.o CoreException.o Message.o ARGS.o Server.o -o $@
serve.o: serve.cpp CoreException.h ARGS.h Server.h Message.h
	$(CC) $(CCFLAGS) -c serve.cpp -o $@

ftrest: thrall.o CoreException.o Message.o ARGS.o Client.o
	$(CC) $(CCFLAGS) thrall.o CoreException.o Message.o ARGS.o Client.o -o $@
thrall.o: thrall.cpp CoreException.h ARGS.h Client.h Message.h
	$(CC) $(CCFLAGS) -c thrall.cpp -o $@

CoreException.o: CoreException.cpp CoreException.h
	$(CC) $(CCFLAGS) -c CoreException.cpp -o $@
Message.o: CoreException.h Message.cpp Message.h ARGS.h
	$(CC) $(CCFLAGS) -c Message.cpp -o $@
ARGS.o: CoreException.h ARGS.cpp ARGS.h
	$(CC) $(CCFLAGS) -c ARGS.cpp -o $@
Server.o: Server.cpp Server.h CoreException.h Message.h
	$(CC) $(CCFLAGS) -c Server.cpp -o $@
Client.o: Client.cpp Client.h CoreException.h Message.h
	$(CC) $(CCFLAGS) -c Client.cpp -o $@


oclean:
	rm -fv *.o

clean:
	rm -fv *.o ftrestd ftrest