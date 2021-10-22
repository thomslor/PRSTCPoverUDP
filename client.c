#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int client_socket;
    int valread;
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr;
    char *msg;
    char *hello = "Hello from client";

    if (argc != 3){
        perror("Not the correct number of args\n");
        exit(EXIT_FAILURE);
    }

    char* ip = argv[1];
    int port = atoi(argv[2]);

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(ip, &serv_addr.sin_addr);

    sendto(client_socket, (const char *) hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
           sizeof(serv_addr));
    printf("Hello message sent.\n");

    close(client_socket);

    return 0;
}