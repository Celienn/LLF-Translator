#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc,char **argv, char **env){
    struct sockaddr_in localaddr ;
    struct sockaddr_in Appelant;    
    char buffer[512];
    int retour;
    printf("test\n");
    int listen_socket = socket(AF_INET,SOCK_STREAM,0);
    localaddr.sin_family = AF_INET; /* Protocole internet */
    /* Toutes les adresses IP de la station */
    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* port d'écoute par défaut au-dessus des ports réservés */
    localaddr.sin_port = htons(9999);
    
    bind(listen_socket,(struct sockaddr*)&localaddr,sizeof(localaddr) );
    
    inet_addr("127.0.0.1");

     
    if (listen(listen_socket,150) == -1) {
        perror("listen error");
    }

    socklen_t Appelantlen = sizeof(Appelant);

    printf(".");
    while(1){
        printf("awaiting a connection.\n");
        Appelantlen = sizeof(Appelant);
        int service = accept(listen_socket,(struct sockaddr*)&Appelant, &Appelantlen);
        if (service == -1){
            printf("accept failed");
            return(1);
        }        

        retour = recv(service,buffer,sizeof(buffer),0);
        if (retour == -1){
            perror("error recv ;");
        }else{
            printf("message sent back : %s\n" , buffer);
        }
         memset(buffer, 0, sizeof(buffer));
    }
        



    return 0;
}