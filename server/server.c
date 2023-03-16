#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

#define PORT            100         // port for tcp-connection
#define IP_ADDR         "127.0.0.2" // ip address for tcp-connection
#define BUF_SIZE        20          // buf size for reading data from socket
#define MAX_CONNECTION  6           // max number of tcp-connection to clients

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   //mutex for locking accessing to list
Node* head = NULL;                                          // list of elements of numbers from clients
 
/*********************************************************************
 *
 * @purpose  Socket Handler
 *
 * @param void *sd              socket descriptor
 *
 * @return void *
 *
 * @note This function for handling for socket-connection.
 *       Read message from client socket. Write data to list
 *
 * @end
 *
 *********************************************************************/
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

/*********************************************************************
 *
 * @purpose  Logger Handler
 *
 * @return void*
 *
 * @note This function for handling for logger.
 *       Read data from list and puts it to console
 *
 * @end
 *
 *********************************************************************/
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


/*********************************************************************
 *
 * @purpose  Main function for server
 *
 * @note Create pthreads for logger and pthread for every new connection for client.
 *
 * @end
 *
 *********************************************************************/
int main(int argc , char *argv[])
{
    int listen_sock;                    // socket for listening
    int client_sock;                    // socket for accepted connection for client
    struct sockaddr_in server;          // socket structure for server
    struct sockaddr_in client;          // socket structure for accepted connection from client
    pthread_t thread_id[MAX_CONNECTION];// array of threads id for handling connetion
    pthread_t thread_log_id;            // thread id for logger
    unsigned int n = 0;                 // number of accepted connection. Connection are not reusable
    int c;                              // size of socket structure

    //init mutex for locking access to list of numbers
    pthread_mutex_init(&mutex, NULL);

    //create pthread for logger
    if( pthread_create( &thread_log_id , NULL ,  loggerHandler , NULL ) < 0)
    {
        perror("Failed to create to thread\n");
        return 1;
    }

    //create listening socket
    listen_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (listen_sock == -1)
    {
        printf("Failed to create to socket\n");
    }
     
    //prepare struct for servers data
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr( IP_ADDR );
    server.sin_port = htons( PORT );
     
    //bind listening socket
    if( bind(listen_sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Failed to bind to socket\n");
        return 1;
    }

    //listen listening socket
    listen(listen_sock , 1);
     
    c = sizeof(struct sockaddr_in);
	
    //connection from clients has been accepted
    while( (client_sock = accept(listen_sock, (struct sockaddr*)&client, (socklen_t*)&c)) > 0 )
    {
        if (client_sock < 0)
        {
            perror("Failed to accept to connection\n");
        }

        if(MAX_CONNECTION > n)
        {
    //create thread for accepted connection
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

    // join pthreads for handling connections
    for(n=0; n< MAX_CONNECTION; n++)
    {
        pthread_join(thread_id[n], NULL);
    }

    // join pthread for logger
    pthread_join(thread_log_id , NULL);

    // free memory for list
    deleteList(&head);
    return 0;
}
