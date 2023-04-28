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
    int sock = -1;                      //local socket for connection
    int write_size = 0;                 //size for writing
    unsigned long tid;                  //thread id

    struct timespec tw = {1,125000000}; //time structure for sleeping
    struct timespec tr;

    tid = pthread_self();

    while(1)
    {
        unsigned int r;
        unsigned int seed = time(NULL) * tid;

        if(sock < 0)
        {
            //start connection on socket
            startConnection(&sock);
        }
        else
        {
        //generate random number
        r = rand_r(&seed);
        r = (r % 100000);

        //write message to socket
        if((write_size = send(sock , (const void*)&r , sizeof (unsigned int), MSG_NOSIGNAL)) < 0)
        {
            if((0 == write_size)||(-1 == write_size))//server disconnected;
            {
                close(sock);
                sock = -1;
            }
        }
        }
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
