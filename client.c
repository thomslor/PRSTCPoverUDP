#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

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
    
    // printf("Buffer avant comparaison SYNACK : %s", buffer);
    if(strcmp(buffer,"SYN-ACK")==0){
        printf("Receive SYN-ACK\n");
        sendto(client_socket, (const char *) ack, strlen(ack),
               MSG_CONFIRM, (const struct sockaddr *) serv_addr,
               sizeof(*serv_addr));
        printf("SYN-ACK recievd, ACK message sent\n");
        if (recvfrom(client_socket, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)serv_addr, &tailleaddr) < 0){
            printf("Couldn't receive Port Number\n");
            return -1;
        }
        return (int) strtol(buffer, NULL, 10);
    }else{
        printf("Recieve not a SYN-ACK\n");
        return -1;
    }
}

int sendData(int client_socket, struct sockaddr_in* serv_addr, char* data, socklen_t tailleaddr){
    sendto(client_socket, (const char *) data, strlen(data),
           MSG_CONFIRM, (const struct sockaddr *) serv_addr,
           sizeof(*serv_addr));
    printf("Data message sent : %s.\n", data);
    return 0;
}

int main(int argc, char* argv[]) {
    int client_socket;
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr;
    struct sockaddr_in serv_addr_data;
    char *hello = "uwu uwu";
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

    memset((char *) &serv_addr_data, 0, sizeof(serv_addr_data));
    serv_addr_data.sin_family = AF_INET;
    serv_addr_data.sin_port = htons(connectionOverUDP(client_socket, &serv_addr, buffer, syn, ack, tailleaddr));
    inet_aton(ip, &serv_addr_data.sin_addr);
    sendData(client_socket, &serv_addr_data, hello, tailleaddr);
    

    close(client_socket);

    return 0;
}