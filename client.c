#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

#define server_path  "server_socket"

void error(char *message);

int main(){
    
    int client_socket;
    char client_name[256] = {};
    printf("Enter your name : ");
    fgets(client_name,256,stdin);  
     while(1){
     
    client_socket =  socket(AF_UNIX, SOCK_STREAM,0);
      if(client_socket == -1){
        error("Error in opening socket \n");
    }
    
    struct sockaddr_un server_address;

    server_address.sun_family = AF_UNIX;
    sprintf(server_address.sun_path, "%s", server_path);
    int client_connection = connect(client_socket,(struct sockaddr *)&server_address, sizeof(server_address));
    
    if(client_connection==-1){
        error("Cannot connect to server \n");
    }

    // if(send(client_socket,client_name,strlen(client_name),0)==-1) error("Error server didn't respond");
    printf("Connected to Server \n");
    printf("Enter the message :");
    char buffer[256] = {};

    fgets(buffer,255,stdin);
    // printf("%s\n",buffer);
    // printf("%d\n",strcmp(buffer, "exit") );
    if(strcmp(buffer, "exit")==10){
         break;
    }
    int s_client = send(client_socket,buffer,strlen(buffer),0);
    if(s_client==-1){
        error("Error writing to client socket");

    }
    char s_response[256];
   if(recv(client_socket,s_response,255,0) < 0) error("Error recieving repsonse from server");
   // printf(" %s\n",s_response);

     }
    close(client_socket);
    return 0;
}


void error(char *message){
    perror(message);
    exit(0);
}