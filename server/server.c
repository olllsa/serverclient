#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

#define PORT            100
#define IP_ADDR         "127.0.0.2"
#define BUF_SIZE        20
#define MAX_CONNECTION  6

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


Node* head = NULL;
 
void *socketHandler(void *sd)
{
    int sock = *(int*)sd;
    int read_size;
    char client_message[BUF_SIZE];

    //Receive a message from client
    while( (read_size = recv(sock , client_message , sizeof (client_message) , 0)) > 0 )
    {
        int i = atoi(client_message);
        pthread_mutex_lock(&mutex);
        push(&head, i);
        pthread_mutex_unlock(&mutex);
        memset(client_message, 0, sizeof (client_message));
    }

    if(read_size == 0)
    {
        //("Client disconnected");
    }
    else if(read_size == -1)
    {
        perror("Failed to receive data from socket");
    }
    close(sock);
    pthread_exit(NULL);
}

void *loggerHandler()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if(head)
        {
            int d = pop (&head);
            char tmp[10] = {0};

            snprintf(tmp, sizeof(tmp), "%d\n", d);
            puts(tmp);
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
 
int main(int argc , char *argv[])
{
    int listen_sock , client_sock , c;
    struct sockaddr_in server , client;
    pthread_t thread_id[MAX_CONNECTION], thread_log_id;
    unsigned int n = 0;

    pthread_mutex_init(&mutex, NULL);

    if( pthread_create( &thread_log_id , NULL ,  loggerHandler , NULL ) < 0)
    {
        perror("Failed to create to thread\n");
        return 1;
    }

    listen_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (listen_sock == -1)
    {
        printf("Failed to create to socket\n");
    }
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr( IP_ADDR );
    server.sin_port = htons( PORT );
     
    if( bind(listen_sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Failed to bind to socket\n");
        return 1;
    }

    listen(listen_sock , 1);
     
    c = sizeof(struct sockaddr_in);
	
    while( (client_sock = accept(listen_sock, (struct sockaddr *)&client, (socklen_t*)&c)) > 0 )
    {
        if (client_sock < 0)
        {
            perror("Failed to accept to connection\n");
        }

        if(MAX_CONNECTION > n)
        {
            if( pthread_create( &thread_id[n++] , NULL ,  socketHandler , (void*) &client_sock) < 0)
            {
                perror("Failed to create to thread\n");
            }
        }
        else
        {
            close(client_sock);
        }
    }

    for(n=0; n< MAX_CONNECTION; n++)
    {
        pthread_join(thread_id[n], NULL);
    }

    pthread_join(thread_log_id , NULL);
    deleteList(&head);
    return 0;
}
