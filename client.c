#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
/*
int connectionOverUDP(int client_socket, struct sockaddr_in* serv_addr, char* buffer, char* syn, char* ack, socklen_t tailleaddr){
    sendto(client_socket, (const char *) syn, strlen(syn),
           MSG_CONFIRM, (const struct sockaddr *) serv_addr,
           sizeof(*serv_addr));
    printf("SYN message sent.\n");

    if (recvfrom(client_socket, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)serv_addr, &tailleaddr) < 0){
        printf("Couldn't receive SYN-ACK\n");
        return -1;
    }
    //printf("Buffer avant comparaison SYNACK : %s", buffer);
    if(strcmp(buffer,"SYN-ACK")==0){
        sendto(client_socket, (const char *) ack, strlen(ack),
               MSG_CONFIRM, (const struct sockaddr *) serv_addr,
               sizeof(*serv_addr));
        printf("SYN-ACK recievd, ACK message sent\n");
        sendto(client_socket, (const char *) hello, strlen(hello),
               MSG_CONFIRM, (const struct sockaddr *) serv_addr,
               sizeof(*serv_addr));
        return 0;
    }else{
        printf("Recieve not a SYN-ACK\n");
        return -1;
    }
}
*/
int main(int argc, char* argv[]) {
    int client_socket;
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr;
    char *hello = "uwu uwu uwu uwu uwu uwu uwu uwu";
    char *syn = "SYN";
    char *ack = "ACK";

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

    socklen_t tailleaddr = sizeof(serv_addr);

    sendto(client_socket, (const char *) syn, strlen(syn),
           MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
           sizeof(serv_addr));
    printf("SYN message sent.\n");

    if (recvfrom(client_socket, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)&serv_addr, &tailleaddr) < 0){
        printf("Couldn't receive SYN-ACK\n");
        return -1;
    }
    //printf("Buffer avant comparaison SYNACK : %s", buffer);
    if(strcmp(buffer,"SYN-ACK")==0){
        sendto(client_socket, (const char *) ack, strlen(ack),
               MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
               sizeof(serv_addr));
        printf("SYN-ACK recievd, ACK message sent\n");
        sendto(client_socket, (const char *) hello, strlen(hello),
               MSG_CONFIRM, (const struct sockaddr *) &serv_addr,
               sizeof(serv_addr));
    }else{
        printf("Recieve not a SYN-ACK\n");
    }

    close(client_socket);

    return 0;
}