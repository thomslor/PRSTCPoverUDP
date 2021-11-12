#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "includes/regex_p.h"


int connectionOverUDP(int client_socket, struct sockaddr_in* serv_addr, socklen_t tailleaddr){
    char buffer[1024] = {0};
    char *syn = "SYN";
    char *ack = "ACK";
    
    sendto(client_socket, (const char *) syn, strlen(syn),
           MSG_CONFIRM, (const struct sockaddr *) serv_addr,
           sizeof(*serv_addr));
    printf("SYN message sent.\n");

    if (recvfrom(client_socket, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)serv_addr, &tailleaddr)<0){
        printf("Couldn't receive SYN-ACK\n");
        return -1;
    }

    printf("Buffer avant comparaison SYNACK : %s\n", buffer);
    if(compareString(buffer, "^SYNACK:[0-9]+$") == 1){
        printf("Receive SYN-ACK  : %s\n", buffer);
        printf("Résult strtok : %s\n", strtok(buffer, ":"));
        int port_data = atoi(strtok(NULL, ":"));
        printf("Port data : %d\n", port_data);
        sendto(client_socket, (const char *) ack, strlen(ack),
               MSG_CONFIRM, (const struct sockaddr *) serv_addr,
               sizeof(*serv_addr));
        printf("SYN-ACK recievd, ACK message sent\n");
        return port_data;
    }else{
        printf("Recieve not a SYN-ACK\n");
        return -1;
    }
}


int sendData(int client_socket, struct sockaddr_in* serv_addr, char* data, socklen_t tailleaddr){
    int send = sendto(client_socket, (const char *) data, strlen(data),
           MSG_CONFIRM, (const struct sockaddr *) serv_addr,
           sizeof(*serv_addr));
    printf("Data message sent with %d caracters : %s.\n", send, data);
    return 0;
}


int main(int argc, char* argv[]) {
    int client_socket;
    struct sockaddr_in serv_addr;
    struct sockaddr_in serv_addr_data;
    char *hello = "Bonjour";
    

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

    int port_data = connectionOverUDP(client_socket, &serv_addr, tailleaddr);
    if (port_data == -1){
        printf("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Port data serveur : %d\n", port_data);


    memset((char *) &serv_addr_data, 0, sizeof(serv_addr_data));
    serv_addr_data.sin_family = AF_INET;
    serv_addr_data.sin_port = htons(port_data);
    inet_aton(ip, &serv_addr_data.sin_addr);
    
    sendData(client_socket, &serv_addr_data, hello, tailleaddr);
    
    close(client_socket);

    return 0;
}