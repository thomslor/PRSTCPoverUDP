#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int SYNC(int serv_socket, struct sockaddr_in* client_addr, char* buffer, char*synack, socklen_t tailleaddr){
    if (recvfrom(serv_socket, buffer, sizeof(buffer), 0,
                 (struct sockaddr*)client_addr, &tailleaddr) < 0){
        printf("Couldn't receive SYN\n");
        return -1;
    }
    char* ntoa = inet_ntoa(client_addr->sin_addr);
    printf("SYN received from %s\n", ntoa);
    int ntoh = ntohs(client_addr->sin_port);
    printf("SYN received from port %d\n", ntoh);

    printf("First Msg from client: %s\n", buffer);

    if (strcmp(buffer,"SYN")==0){
        
        sendto(serv_socket, (const char *) synack, strlen(synack),
               MSG_CONFIRM, (const struct sockaddr *) client_addr,
               tailleaddr);
        printf("Send SYNACK\n");

        if (recvfrom(serv_socket, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)client_addr, &tailleaddr) < 0){
            printf("Couldn't receive ACK\n");
            return -1;
        }
        if (strcmp(buffer,"ACK")==0){
            if (recvfrom(serv_socket, buffer, sizeof(buffer), 0,
                         (struct sockaddr*)client_addr, &tailleaddr) < 0){
                printf("Couldn't receive first message\n");
                return -1;
            }
            printf("Msg from client: %s\n", buffer);
        }else{
            printf("Recieve not an ACK");
        }

    }else{
        printf("Recieve not a SYN");
    }
    return 0;
}


int main(int argc, char* argv[]) {
    int serv_socket = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1;
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr_2;
    struct sockaddr_in client_addr;
    char *synack = "SYN-ACK";

    if (argc != 2){
        perror("Not the correct number of args : Give the server port\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    if (serv_socket<0){
        perror("Socket UDP creation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Socket Serveur : %d\n", serv_socket);
    printf("Port Serveur : %d\n", port);

    setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    memset((char*)&serv_addr_2,0,sizeof(serv_addr_2));
    serv_addr_2.sin_family = AF_INET;
    serv_addr_2.sin_port = htons(port);
    serv_addr_2.sin_addr.s_addr = INADDR_ANY;

    if(bind(serv_socket, (struct sockaddr *)&serv_addr_2,sizeof(serv_addr_2))<0){
        perror("Bind UDP failed");
        exit(EXIT_FAILURE);
    }

    socklen_t tailleaddr = sizeof(client_addr);
    while (1){
        SYNC(serv_socket, &client_addr, buffer, synack, tailleaddr);


    }
    close(serv_socket);




}