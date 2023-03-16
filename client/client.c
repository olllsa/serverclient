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
 
int startConnection(int *s)
{
    struct sockaddr_in server;
    *s = socket(AF_INET , SOCK_STREAM , 0);
    if (*s == -1)
    {
        //("Filed to create to socket\n");
        return *s;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr( IP_ADDR );
    server.sin_port = htons( PORT );

    int status = connect(*s, (struct sockaddr *) &server, sizeof(server));
    if (status == -1)
    {
        //("Failed to connect to server\n");
        return status;
    }
    return *s;
}
void *clientHandler()
{
    int sock = 0;
    int write_size = 0;
    char client_message[BUF_SIZE] = {0};

    struct timespec tw = {1,125000000};
    struct timespec tr;
    unsigned long tid;

    tid = pthread_self();

    startConnection(&sock);

    while(sock)
    {
        unsigned int r;
        unsigned int seed = time(NULL) * tid;

        r = rand_r(&seed);
        r = (r % 100000);

        snprintf(client_message, sizeof(client_message), "%d\n", r);
        if((write_size = write(sock , client_message , strlen(client_message))) < 0)
        {
            if(write_size == 0)
            {
                //("Server disconnected\n");
                close(sock);
                startConnection(&sock);
            }
            else if(write_size == -1)
            {
                close(sock);
                perror("Filed to write to socket\n");
            }
        }
        tw.tv_nsec = r;
        memset(client_message, 0, sizeof (client_message));
        nanosleep (&tw, &tr);
    }

    close(sock);
    return 0;
}


int main(int argc , char *argv[])
{
    pthread_t thread_id[CLIENTS_NUM];

    for(int i =0; i < CLIENTS_NUM; i++)
    {
        if( pthread_create( &thread_id[i] , NULL , clientHandler, NULL) < 0)
        {
            perror("Filed to create thread\n");
            return 1;
        }
    }

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
 

