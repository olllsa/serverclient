#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#define CLIENTS_NUM 4
#define IP_ADDR     "127.0.0.2"
#define BUF_SIZE    13
#define PORT        100
 
/*********************************************************************
 *
 * @purpose  Start connection on sockets
 *
 * @return void*
 *
 * @note This function for starting connection on socket
 *
 * @end
 *
 *********************************************************************/
int startConnection(int *s)
{
    struct sockaddr_in server;

    //create the socket
    *s = socket(AF_INET , SOCK_STREAM , 0);
    if (*s == -1)
    {
        puts("Filed to create to socket\n");
        return *s;
    }

    //prepare socket structure with servers parameters
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr( IP_ADDR );
    server.sin_port = htons( PORT );

    //connect to socket
    int status = connect(*s, (struct sockaddr *) &server, sizeof(server));
    if (status == -1)
    {
        puts("Failed to connect to server\n");
        return status;
    }
    return *s;
}

/*********************************************************************
 *
 * @purpose  Client Handler
 *
 * @return void*
 *
 * @note This function for handling client.
 *       Generates random number and write it to connected socket
 *
 * @end
 *
 *********************************************************************/
void *clientHandler()
{
    int sock = 0;                       //local socket for connection
    int write_size = 0;                 //size for writing
    char client_message[BUF_SIZE] = {0};//buffer for message to server
    unsigned long tid;                  //thread id

    struct timespec tw = {1,125000000}; //time structure for sleeping
    struct timespec tr;

    tid = pthread_self();
    //start connection on socket
    startConnection(&sock);

    while(sock)
    {
        unsigned int r;
        unsigned int seed = time(NULL) * tid;

        //generate random number
        r = rand_r(&seed);
        r = (r % 100000);

        //convert random number to message
        snprintf(client_message, sizeof(client_message), "%d\n", r);
        //write message to socket
        if((write_size = write(sock , client_message , strlen(client_message))) < 0)
        {
            if(write_size == 0)//server disconnected;
            {
                close(sock);
                startConnection(&sock);
            }
            else if(write_size == -1)
            {
                close(sock);
                perror("Filed to write to socket\n");
            }
        }
        //clean client message
        memset(client_message, 0, sizeof (client_message));
        //sleep some time
        tw.tv_nsec = r;
        nanosleep (&tw, &tr);
    }

    close(sock);
    return 0;
}

/*********************************************************************
 *
 * @purpose  Main function for client
 *
 *
 * @note This function creates pthread for clients
 *
 * @end
 *
 *********************************************************************/
int main(int argc , char *argv[])
{
    pthread_t thread_id[CLIENTS_NUM];   //array of thread ids for clients pthreads

    //create pthreads for clients
    for(int i =0; i < CLIENTS_NUM; i++)
    {
        if( pthread_create( &thread_id[i] , NULL , clientHandler, NULL) < 0)
        {
            perror("Filed to create thread\n");
            return 1;
        }
    }

    //join pthreads for clients
    for(int i =0; i < CLIENTS_NUM; i++)
    {
        if( pthread_join( thread_id[i] , NULL ) < 0)
        {
            perror("Filed join thread\n");
            return 1;
        }
    }

    return 0;
}
