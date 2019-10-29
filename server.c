#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#define path "server_socket"


char *remove_spaces(char buffer[256]);
void error(char *message);
struct client{
    int client_socket ;
    char *name;
    struct sockaddr_un client_address;
};

int main(int argc, char *argv[]){

    int max_clients = atoi(argv[1]);
    int all_clients[max_clients] ;
    for(int j=0;j<max_clients;j++){
        all_clients[j] = 0;
    }
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
  
   
    while(1){
         if(listen(server_socket,max_clients) == -1){
        error("Error in listening via socket");
    }
        client_socket = 0;
    char server_message[256] = {};
    for(int j=0;j<max_clients;j++){
        printf("%d\n",all_clients[j]);
    }
    int len = sizeof(address_client);
    client_socket = accept(server_socket,(struct sockaddr *)&address_client,&len);
    if(client_socket == -1){
        error("Error cannot connect to client \n");
    }
    
    char x[256] ;
        
    // int check_st = recv(client_socket,x,256,0);
    int i=0;
    int done = 0;
    for(i=0;i<max_clients;i++){
        if(client_socket == all_clients[i]){
            done=1;
        }
    }
    // printf("%s\n",x);
    if(!done){
         for(i=0;i<max_clients;i++){
        if(all_clients[i] == 0){
            
           all_clients[i] = client_socket;
           break;
        }
    }
    }

    // for(i=0;i<max_clients;i++){
    //     if(all_clients[i].name !=NULL){
    //         printf("%s ,%d\n", all_clients[i].name,all_clients[i].client_socket);}
        
    // }
    

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

