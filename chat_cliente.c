#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

int main(){
    struct addrinfo socketConfig;
    memset(&socketConfig, 0, sizeof(socketConfig));
    
    struct addrinfo *socketList;
    int status_init, socket_cliente;
    char address[256],port[256];

    socketConfig.ai_family = AF_INET;
    socketConfig.ai_socktype = SOCK_STREAM;
    socketConfig.ai_flags = AI_PASSIVE;
    system("clear");

    printf("Enter the ipv4 of the host: ");
    fgets(address,256,stdin);
    address[strlen(address)-1] ='\0';
    system("clear");

    printf("Enter the port that the server is listening (E.g: 8080)\nport: ");
    fgets(port,256,stdin);
    port[strlen(port)-1] = '\0';

    status_init = getaddrinfo(address, port, &socketConfig, &socketList);
    socket_cliente = socket(socketList->ai_family,socketList->ai_socktype,socketList->ai_protocol);
    
    if (connect(socket_cliente, socketList->ai_addr, socketList->ai_addrlen) == -1){
        printf("Connection failed, verify the port inputed \nand if this computer is in the same network as the host");
        exit(1);
        close(socket_cliente);
    }

    system("clear");
    printf("Client connected, you can start sending messages, now!\n");

    while (1) {
        char buffer[2000];
        memset(buffer, 0, 2000);
        printf("\n-> ");
        fgets(buffer, 2000, stdin);
        send(socket_cliente, buffer, strlen(buffer), 0);
    }
}