COMPILER = gcc
OPTIONS = -c -g -Wall

all: cliente servidor

servidor: io.o servidor.o
	$(COMPILER) io.o servidor.o -o servidor -lm

cliente: io.o cliente.o
	$(COMPILER) io.o cliente.o -o cliente -lm

servidor.o: servidor.c
	$(COMPILER) $(OPTIONS) servidor.c

cliente.o: cliente.c
	$(COMPILER) $(OPTIONS) cliente.c

io.o: io.c
	$(COMPILER) $(OPTIONS) io.c

clean:
	rm -f *.o io cliente servidor