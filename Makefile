CFLAGS	+= -Wall   -g
LDFLAGS += -Wall

all: client server

regex_p.o: regex_p.c includes/regex_p.h
	gcc -c regex_p.c -I /includes -o regex_p.o

client.o: client.c
	gcc ${CFLAGS} -c client.c -o client.o

client:  client.o  regex_p.o
	gcc ${LDFLAGS}  client.o regex_p.o -o client

serveur.o: server.c
	gcc ${CFLAGS} -c server.c -o server.o

server: server.o
	gcc ${LDFLAGS}  server.o -o server

clean:
	-rm *.o client server