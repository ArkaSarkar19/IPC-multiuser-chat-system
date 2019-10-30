#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#define path "server_socket"
struct client all_clients[100];
int max_clients = 0;

pthread_t client_threads[100];
char *remove_spaces(char buffer[256]);
void error(char *message);
char ** getdata(char buffer[256]);
void server(int max_client);

struct client{
    int source_client_socket ;
    int dest_client_socket;
    struct sockaddr_un client_address;
};

int main(){
    //  max_clients = atoi(argv[1]);
    // for(int j=0;j<max_clients;j++){
    //     all_clients[j] = 0;
    // }
    int server_socket,client_socket;
    
    server_socket = socket(AF_UNIX, SOCK_STREAM,0);
    if(server_socket == -1){
        error("Error in opening socket \n");
    }
    printf("%d\n",server_socket);
    struct sockaddr_un address_server,address_client;
    address_server.sun_family = AF_UNIX;
    
    strcpy(address_server.sun_path,path);
    unlink(address_server.sun_path);
    int server_bind = bind(server_socket,(struct sockaddr *) &address_server,  sizeof(address_server));
    if(server_bind == -1){
        error("Error establishing a server socket connection \n");
    }
  if(listen(server_socket,10) == -1){
        error("Error in listening via socket");
    }
    while(1){

    // for(int j=0;j<max_clients;j++){
    //     printf("%d\n",all_clients[j]);
    // }
    int len = sizeof(address_client);
    client_socket = accept(server_socket,(struct sockaddr *)&address_client,&len);
    printf("current client socket %d\n",client_socket);
    if(client_socket == -1){
        error("Error cannot connect to client \n");
    }
    all_clients[max_clients].source_client_socket = client_socket;

    
    pthread_create(&client_threads[max_clients],NULL,server,max_clients);
    //printf("The messeage recieved is : %s", server_message );
    // char mes[256] = "Message recieved";
    // if(send(client_socket,mes,strlen(mes),0 ) < 0){
    //     error("Error in replying to client");
    // }
    //close(client_socket);
    max_clients++;
    }

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


char ** getdata(char buffer[256]){
    int i,j,k = 0;
    int flag1 = 0;
    int flag2 = 0;
    char **data[2][256];
    while(buffer[i]!='\n'){
        if(flag1==0  ){
            data[0][j] = buffer[i];
            j++; 
        }
        else if(flag1==0 && buffer[i]=='|') flag1 = 1;
        else if(flag1 == 1 && flag2==0){
            data[1][k] = buffer[i];
            k++;
        }
        else;
        ++i;
    }
    return data;
}
void server(int max_client){

    while(1){
    char server_message[256] = {};

     int status = recv(all_clients[max_client].source_client_socket,server_message,256,0);
    if(status == -1){
        error("Error in recieving message");
    }
    char **data[2][256] = getdata(server_message);
    all_clients[max_clients].dest_client_socket = atoi(data[0]);
    if(send(all_clients[max_client].dest_client_socket,data[1],strlen(data[1]),0)==-1){
        error("Error send message back to client");
    }
    }
}