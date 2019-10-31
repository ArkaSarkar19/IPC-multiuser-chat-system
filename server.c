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
#include <signal.h>
#define path "server_socket"
int max_clients = 0;
pthread_t thread[100];


char *remove_spaces(char buffer[256]);
void error(char *message);
char ** getdata(char buffer[256]);
void server(int max_client);
void cleanup();
struct client{
    int source_client_socket ;
    int dest_client_socket;
    struct sockaddr_un client_address;
};

static  struct client all_clients[100];

int main(){

    int server_socket,client_socket;
    key_t key = ftok("shmfile",65); 
    char str[256];
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    char *data = (char*) shmat(shmid,(void*)0,0); 
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
  if(listen(server_socket,100) == -1){
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
    signal(SIGINT,cleanup);
    strcat(str, " |");
    char add[10];
    sprintf(add,"%d",client_socket);
    strcat(str, add);
    strcpy(data,str);

    all_clients[max_clients].source_client_socket = client_socket;
    printf("%d\n",all_clients[max_clients].source_client_socket);
    pthread_create(&thread[max_clients],NULL,server,(void *)max_clients);
    max_clients++;
    printf("max clients %d\n", max_clients);
    }
    cleanup(1);
    

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
  // int  m = *((int *)max_client);
    while(1){
    char data1[256] = {};

    //printf("ggggggggggggggggggggggggg%d\n",all_clients[max_client].source_client_socket);
     int status1 = recv(all_clients[max_client].source_client_socket,data1,256,0);
    if(status1 == -1){
        error("Error in recieving message");
    }
    //char **data[2][256] = getdata(server_message);

 char data2[256] = {};

     int status2 = recv(all_clients[max_client].source_client_socket,data2,256,0);
    if(status2 == -1){
        error("Error in recieving message");
    }
    // printf("%s\n",data1);
    // printf("%s\n",data2);
    // int d = atoi(data1);
    // sprintf(d,"%d",data1);
   //  printf("%d\n",atoi(data1));
   printf("%d\n",strcmp(data1,"EVERYONE"));
     if(strcmp(data1,"EVERYONE")==10){
         printf("%d\n",max_clients);
         for(int i=0;i<max_clients;i++){
             printf("ddddddddddddddd%d\n",i);
             printf("%d\n",all_clients[i].source_client_socket);
            if(send(all_clients[i].source_client_socket,data2,strlen(data2),0)==-1){
                error("Error send message1 back to client \n");
            }
         }
     }
     else{
    all_clients[max_client].dest_client_socket = (int)(data1);
   // printf("Destinatinf socket %d\n",all_clients[max_client].dest_client_socket);
    data2[status2] = '\0';
    if(send(atoi(data1),data2,strlen(data2),0)==-1){
        error("Error send message446 back to client\n");
    }
     }
    }
}
void cleanup(int sig){
   // fflush(stdout);
    printf("EXITING \n");
    for(int i=0;i<max_clients;i++){
        pthread_join(thread[i],NULL);
    }
    exit(0);
}