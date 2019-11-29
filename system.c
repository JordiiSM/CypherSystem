//
// Created by Jordi on 12/11/2019.
//
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "system.h"

int socketConnect;

//-----------------------------------LEER TECLADO------------------------------------

void read_keyboard(char *string){
    int i = 0;
    char *buffer = NULL;
    char caracter = ' ';
    while( caracter != '\n'){

        read(STDIN_FILENO,&caracter, sizeof(char));

        buffer = (char*) realloc (buffer,i+1);

        buffer[i] = caracter;
        //Pasa mayus a minus
        if (caracter >= 'A' && caracter <= 'Z'){
            buffer[i] = caracter - ('A'-'a');
        }
        //Sustituye \n con \0
        if(caracter == '\n'){
            buffer[i] = '\0';
        }

        i++;
    }
    strcpy(string,buffer);
    free(buffer);
}

//-------------------------------------MOSTRAR POR PANTALLA------------------------------

void show(char *string){

    write(STDOUT_FILENO,string, sizeof(char) * strlen(string));

}

void terminal(config *Configuration){
    char buffer[100];
    memset(buffer,'\0',100);
    char command[50];
    int option = 0;
    while (option != 1) {
        option = 0;
        sprintf(buffer, "\n\033[1;31m $%s \033[0m", Configuration->user);

        show(buffer);
        read_keyboard(command);
        option = processCommand(command,Configuration);
    }
    show(buffer);
    show(" \nBye!!\n");
}
int processCommand(char command[50],config *Configuration){
    int option = 0;
    char buffer[50];
    char* puerto = malloc(50*sizeof(char));
    char* user = malloc(50*sizeof(char));
    char* msg = malloc(500* sizeof(char));



    option = analizeCommand(command);
    switch (option){
        case 0:
            show("Command not recognized");
            break;
        case 1:
            showConnections(Configuration);
            break;
        case 2:
            split(1,command,puerto);
            connection(puerto, Configuration);
            sprintf(buffer,"Connectando puerto %s \n",puerto);
            show(buffer);
            break;
        case 3:
            split(1,command,user);
            split(2,command,msg);
            sendMsg(socketConnect,msg);
            sprintf(buffer,"Diciendo a %s %s\n",user,msg);
            show(buffer);
            break;

        case 4:
            split(2,command,user);
            sprintf(buffer,"Mostrando audios de %s\n",user);
            show(buffer);
            break;
        case 5:
            show("Descargando\n");
            break;
        case 6:
            show("Bye!!!\n");
            return 1;

    }
    free(puerto);
    free(user);
    return 0;
}
int analizeCommand(char command[50]){

    char buffer[50];
    memset(buffer,'\0',50);
    if (strcmp(command, "show connections") == 0){
        return 1;
    }
    if (strcmp(command, "exit") == 0){
        return 6;
    }
    for(int i = 0;i<30;i++){

        buffer[i] = command[i];
//        show(buffer);
//        show("\n");
        if(strcmp(buffer,"connect")==0){
                return 2;
        }else if(strcmp(buffer,"say")==0){
                return 3;
            }else if(strcmp(buffer,"show audios")==0){
                return 4;
            }else if(strcmp(buffer,"download")==0){
            return 5;
        }

    }
    return 0;
}
 void split (int nword, char text[50],char* splitted){
    char nsplit = 0;
    int j = 0;
    for(int i=0;i<50;i++){
        if(nsplit == nword || (nword ==2 && nsplit > 2)){
            splitted[j] = text[i];
            j++;

        }
        if(text[i] == ' '){
            nsplit++;
        }


    }
}

void showConnections(config *Configuration){
    int connectionFork;

    char buffer[100];

    connectionFork = fork();
    if (connectionFork > 0) { //-------------------padre
        sleep(1);
    }
    if (connectionFork == 0) {

        char *portini = malloc(sizeof(char));
        char *portfin = malloc(sizeof(char));
        itoa(Configuration->connectionPortIni,portini);
        itoa(Configuration->connectionPortFin,portfin);
        sprintf(buffer, "\n\033[0;33mAVAILABLE CONNECTIONS %s / %s \n\033[0m",portini,portfin);
        show(buffer);
        char *argv;
        argv[0] = "./show_connections.sh";
        argv[1] = portini;
        argv[2] = portfin;
        argv[3] = NULL;

        execv("show_connections.sh",argv);
    }
}
//-------------------Conecta socket-----------------------------
void itoa(int num,char* buff){
    sprintf(buff,"%d",num);
}
int conectionSocket(int port){

    struct in_addr ip_addr;
    if (inet_aton ("127.0.0.1", &ip_addr) == 0){ //ip
        fprintf (stderr, "inet_aton (%s): %s\n", "127.0.0.1", strerror (errno)); //ip
        exit (EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror ("socket TCP");
        exit (EXIT_FAILURE);
    }
    struct sockaddr_in s_addr;
    bzero (&s_addr, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (port);
    s_addr.sin_addr = ip_addr;

    if (connect (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0)
    {
        perror ("connect");
        exit (EXIT_FAILURE);
    }
    return sockfd;

}

//

void sendMsg(int socket, char data[30]){
    trama msg;
    msg.type = 0x02;
    strcpy(msg.header,"[MSG]");
    msg.length = strlen(data);
    strcpy(msg.data,data);
    write (socket, msg.type, sizeof (char));
    write (socket, msg.header, sizeof (msg.header));
    write (socket, msg.length, sizeof (int));
    write (socket, msg.data, sizeof (msg.data));
}
void connection(int puerto,config *Configuration){
    trama msg;
    socketConnect = conectionSocket(atoi(puerto))
    msg.type = 0x01;
    strcpy(msg.header,"[TR_NAME]");
    msg.length = strlen(Configuration->user);
    strcpy(msg.data,Configuration->user);
    write (socket, msg.type, sizeof (char));
    write (socket, msg.header, sizeof (msg.header));
    write (socket, msg.length, sizeof (int));
    write (socket, msg.data, sizeof (msg.data));
}




