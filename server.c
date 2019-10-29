#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

char *remove_spaces(char buffer[256]);
void error(char *message);
#define path "server_socket"

int main(int argc, char *argv[]){
    int max_clients = atoi(argv[1]);
   struct  client all_clients[max_clients];
    int server_socket,client_socket;
    
    server_socket = socket(AF_UNIX, SOCK_STREAM,0);
    if(server_socket == -1){
        error("Error in opening socket \n");
    }
    struct sockaddr_un address_server,address_client;
    address_server.sun_family = AF_UNIX;
    
    strcpy(address_server.sun_path,path);
    unlink(address_server.sun_path);
    int server_bind = bind(server_socket,(struct sockaddr *) &address_server,  sizeof(address_server));
    if(server_bind == -1){
        error("Error establishing a server socket connection \n");
    }
    listen(server_socket,max_clients);
    if(listen == -1){
        error("Error in listening via socket");
    }
    while(1){
            char server_message[256] = {};

        int len = sizeof(address_client);
    client_socket = accept(server_socket,(struct sockaddr *)&address_client,&len);
    if(client_socket == -1){
        error("Error cannot connect to client \n");
    }
    int i=0;
    for(i=0;i<max_clients;i++){
        
    }
    int status = recv(client_socket,server_message,256,0);
    if(status == -1){
        error("Error in recieving message");
    }
    printf("The messeage recieved is : %s", server_message );
    char mes[256] = "Message recieved";
    if(send(client_socket,mes,status,0 ) < 0){
        error("Error in replying to client");
    }
    close(client_socket);
    }
    return 0;
}


void error(char *message){
    perror(message);
    exit(0);
}

char * remove_spaces(char buffer[256]){
    int i = 0;
    char *str = "";
    while(buffer[i]!='\n'){
        if(buffer[i]!=' '){
            strcat(str,buffer[i]);
        }
        i++;
    }
    return str;
}

struct client{
    int client_socket;
    char *name;
    struct sockaddr_un client_address;
};