COMPILER = gcc
OPTIONS = -c -g -Wall

all: client server

client: io.o showip.o time.o client.o
	$(COMPILER) io.o showip.o time.o client.o -o client

server: io.o showip.o time.o server.o
	$(COMPILER) io.o showip.o time.o server.o -o server

client.o: client.c
	$(COMPILER) $(OPTIONS) client.c

server.o: server.c
	$(COMPILER) $(OPTIONS) server.c
	
io.o: io.c
	$(COMPILER) $(OPTIONS) io.c

showip.o: showip.c		
	$(COMPILER) $(OPTIONS) showip.c
	
time.o: time.c
	$(COMPILER) $(OPTIONS) time.c
	
clean:
	rm -f *.o client server io showip time

