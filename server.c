
#include "server.h"
#include "system.h"


void *sockThread(void *arg){
    int newsock = *((int *)arg);
    ssize_t len;
    char buff[513];
    do{
        len = read (newsock, buff, 512);
        buff[len] = 0;
        printf ("%s\n", buff);
    }while (strcmp(buff,"exit\n") != 0);
    printf("Me piro");
    close (newsock);
    return EXIT_SUCCESS;
}
void createServer (int port){
    int connectionFork;
    connectionFork = fork();
    if (connectionFork > 0) { //-------------------padre
        sleep(1);
    }
    if (connectionFork == 0) {
        pthread_t *thread;
        thread = malloc(sizeof(pthread_t));
        int i = 0;
        int *newsock;
        newsock = malloc(sizeof(int));
        int estat;
        int sockfd;
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd < 0) {
            perror("socket TCP");
            exit(EXIT_FAILURE);
        }
        struct sockaddr_in s_addr;
        bzero(&s_addr, sizeof(s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(port);
        s_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (void *) &s_addr, sizeof(s_addr)) < 0) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        listen(sockfd, 5);

        do {
            struct sockaddr_in c_addr;
            socklen_t c_len = sizeof(c_addr);

            newsock = malloc(sizeof(int));
            newsock[i] = accept(sockfd, (void *) &c_addr, &c_len);
            printf("New connection from %s:%hu\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
            estat = pthread_create(&thread[i], NULL, sockThread, &newsock[i]);
            if(estat !=0){
                show("Error thread\n");
            }

            i++;
            if (*newsock < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            //Funcion cliente

        } while (1);
    }
    //hacer joins
    //hacer frees

}