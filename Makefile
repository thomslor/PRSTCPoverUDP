CFLAGS	+= -Wall   -g
LDFLAGS += -Wall

all: client server

client.o: client.c
	gcc ${CFLAGS} -c client.c -o client.o

client:  client.o
	gcc ${LDFLAGS}  client.o -o client

serveur.o: server.c
	gcc ${CFLAGS} -c server.c -o server.o

server: server.o
	gcc ${LDFLAGS}  server.o -o server

clean:
	-rm *.o client server