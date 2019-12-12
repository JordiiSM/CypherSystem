
#include "server.h"
void checkTrams(trama t,int socketTemp){
    trama answer;

    if((int)t.type == 0x01){
        char buffer[50];
        read (socketTemp, t.header, sizeof(char)*strlen("[TR_NAME]"));
        read (socketTemp, &t.length, sizeof(unsigned short));
        t.data = malloc(sizeof(char)*t.length);
        read (socketTemp, t.data, sizeof(char)*t.length);
        sleep(1);

        show(t.data);
        show(" se ha conectado\n");
        answer.type = 0x01;
        answer.header = malloc(sizeof(char)*7);
        strcpy(answer.header,"[CONOK]");
        answer.length = (unsigned short)strlen(Configuration.user);
        answer.data = malloc(sizeof(Configuration.user));
        strcpy(answer.data,Configuration.user);

        write (socketTemp, &answer.type, sizeof (char));
        write (socketTemp, answer.header, sizeof (char)*strlen("[CONOK]"));
        write (socketTemp, &answer.length, sizeof (unsigned short));
        write (socketTemp, answer.data, sizeof (char)*answer.length);//
        sleep(1);
        sprintf(buffer, "\n\033[1;31m $%s \033[0m", Configuration.user);
        show(buffer);

    }else if((int)t.type == 0x02){
            char buffer[50];
            read (socketTemp, t.header, sizeof(char)*strlen("[MSG]"));
            read (socketTemp, &t.length, sizeof(unsigned short));
            t.data = malloc(sizeof(char)*t.length);
            read (socketTemp, t.data, sizeof(char)*t.length);
            sleep(1);
            answer.type = 0x02;
            answer.header = malloc(sizeof(char)*7);
            strcpy(answer.header,"[MSGOK]");
            answer.length = 0;

            show("\nHas recibido: ");
            show(t.data);
            show("\n");

            write (socketTemp, &answer.type, sizeof (char));
            write (socketTemp, answer.header, sizeof (char)*strlen("[MSGOK]"));
            write (socketTemp, &answer.length, sizeof (unsigned short));
            write (socketTemp, answer.data, sizeof (char)*answer.length);//

            sprintf(buffer, "\n\033[1;31m $%s \033[0m", Configuration.user);
            show(buffer);

    }else if((int)t.type == 0x06){
        char buffer[50];
        read (socketTemp, t.header, sizeof(char)*strlen("[]"));
        read (socketTemp, &t.length, sizeof(unsigned short));
        t.data = malloc(sizeof(char)*t.length);
        read (socketTemp, t.data, sizeof(char)*t.length);
        show(t.data);
        show(" se ha desconectado\n");

        sprintf(buffer, "\n\033[1;31m $%s \033[0m", Configuration.user);
        show(buffer);
//        answer.type = 0x06;
//        answer.header = malloc(sizeof(char)*7);
//        strcpy(answer.header,"[CONKO]");
//        answer.length = 0;

    }
}

void *sockThread(void *arg){

    int newsock = *((int *)arg);
    trama t;
    t.header = malloc(sizeof(char)*10);
    do{
        //printf("Haciendo reads\n");
        read (newsock, &t.type, sizeof(char));
        checkTrams(t,newsock);
        sleep(1);
    }while (t.type != 0x06); //avis signal
    close (newsock);
    return EXIT_SUCCESS;
}
void *createServer (){

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
        s_addr.sin_port = htons(Configuration.port);
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
            //printf("New connection from %s:%hu\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
            estat = pthread_create(&thread[i], NULL, sockThread, &newsock[i]);
            if(estat !=0){
                show("Error thread\n");
            }

            i++;
            if (*newsock < 0) {
                perror("accept");
            }
            //Funcion cliente

        } while (1);
    //hacer frees

}