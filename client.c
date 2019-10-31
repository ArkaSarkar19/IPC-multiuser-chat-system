#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/shm.h>
#define server_path  "server_socket"

void error(char *message);
void listen1(int client_socket);
int main(){
    pthread_t thread;
    int client_socket;
    // char client_name[256] = {};
    // printf("Enter your name : ");
    // fgets(client_name,256,stdin);  
    key_t key = ftok("shmfile",65); 
  
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  

    client_socket =  socket(AF_UNIX, SOCK_STREAM,0);
    printf("%d\n",client_socket);
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
    pthread_create(&thread,NULL,listen1,(void *)client_socket);
    while(1){
    char c[100] = {};
    fgets(c,100,stdin);
    char *str = (char*) shmat(shmid,(void*)0,0);
    printf("Avalable clients : %s\n", str);

    printf("Enter the client to send:");
    char buffer1[256] = {};

    fgets(buffer1,256,stdin);
    // printf("%s\n",buffer);
    // printf("%d\n",strcmp(buffer, "exit") );
    if(strcmp(buffer1, "exit")==10){
         break;
    }
    int s_client1 = send(client_socket,buffer1,strlen(buffer1),0);
    if(s_client1==-1){
        error("Error writing to client socket");

    }
    printf("Enter the message:");

    char buffer2[256] = {};

    fgets(buffer2,256,stdin);
    // printf("%s\n",buffer);
    // printf("%d\n",strcmp(buffer, "exit") );
    if(strcmp(buffer2, "exit")==10){
         break;
    }
    int s_client2 = send(client_socket,buffer2,strlen(buffer2),0);
    if(s_client2==-1){
        error("Error writing to client socket");

    }


      }
    close(client_socket);
    return 0;
}


void error(char *message){
    perror(message);
    exit(0);
}

void listen1(int client_socket){
    while(1){
        char buffer[256] = {};
        int i;
        if(i= recv(client_socket,buffer,256,0)==-1){
            error("Error in revieving");
        }
    //    buffer[i] = '\0';
     printf("NEW MESSAGE %s\n", buffer);

    }
}