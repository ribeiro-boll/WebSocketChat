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

void listen_clients(int socket,int contador_usuarios){
    pid_t pid = fork();
    if (pid == 0){
        char buffer[2000];
        int status = 1;
        memset(buffer, 0,2000);

        while (1) {
            if (!(status>0)){
                printf("\n***[->User %d<-] left the chat***\n\n",contador_usuarios);
                FILE *arquivo = fopen("connections.txt", "r");
                int contador = fgetc(arquivo) - 1 ;
                fclose(arquivo);

                arquivo = fopen("connections.txt", "w");
                fputc(contador, arquivo);
                fclose(arquivo);

                close(socket);
                exit(0);
            }

            memset(buffer, 0, 2000);
            status = recv(socket, buffer, 2000, 0);

            if (strcmp(buffer,"") == 0){
                continue;
            }
            printf("[->User %d<-]: %s",contador_usuarios,buffer);
        }
    }
    else if (pid == -1) {
        printf("\n\nFork error, User %d, could not join!!!\n\n",contador_usuarios);
    }
    return;
}
int main(){
    struct addrinfo configInicial;
    memset(&configInicial, 0, sizeof(configInicial));

    struct addrinfo *listaSockets;
    struct sockaddr_storage socket_client_config;
    socklen_t arr_size = sizeof(socket_client_config);
    int status_inicio, status_bind;
    int socket_Inicial, socket_client;
    char endereco_ip[256],port[256];

    printf("Enter enter a number of a Port (E.g: 8080)\nport: ");
    while (1){
        fgets(port,256,stdin);
        port[strlen(port)-1]='\0';

        configInicial.ai_flags= AI_PASSIVE;
        configInicial.ai_socktype = SOCK_STREAM;
        configInicial.ai_family = AF_INET;

        status_inicio = getaddrinfo(NULL, port, &configInicial, &listaSockets);
        if (status_inicio == -1){
            printf("failure setting up 'getaddrinfo', verify if the selected port is free!\n\nport: ");
            memset(&configInicial, 0, sizeof(configInicial));
            continue;
        }
        break;
    }
    socket_Inicial = socket(listaSockets->ai_family,listaSockets->ai_socktype,listaSockets->ai_protocol);
    
    if (socket_Inicial == -1) {
        printf("failure setting up 'initial Socket'...");
        close(socket_Inicial);
        exit(1);
    } 
    
    status_bind = bind(socket_Inicial, listaSockets->ai_addr, listaSockets->ai_addrlen);
    
    if (status_bind == -1){
        printf("failure setting up 'bind'...");
        close(socket_Inicial);
        exit(1); 
    }

    system("clear");
    int contador_usuarios = 0;
    printf("*Server up and running!!\n*To get your IPV4, type 'ip a' on your shell, and get the number after 'inet'\n*Example: inet 192.0.10.6/10\n*\n*in the client, only enter the '192.0.10.6'!\n*Waiting for users input...\n_____________________________________________________________\n\n");
    
    FILE *arquivo;
    arquivo = fopen("connections.txt","w");
    fputc('0', arquivo);
    fclose(arquivo);
    
    while(1){
        listen(socket_Inicial, 10);
        socket_client =  accept(socket_Inicial,(struct sockaddr *)&socket_client_config, &arr_size);
        
        arquivo = fopen("connections.txt","r");
        contador_usuarios = (fgetc(arquivo) - '0' + 1);
        fclose(arquivo);
        
        arquivo = fopen("connections.txt","w");
        fputc(contador_usuarios + '0', arquivo);
        fclose(arquivo);

        listen_clients(socket_client, contador_usuarios);
    }
}