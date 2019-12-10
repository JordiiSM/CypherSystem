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

Connections *connectionList;
int connectionsCounter = 0;

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
        case 2: //Connect
            split(1,command,puerto);
            connection(puerto, Configuration);
            show(buffer);
            break;
        case 3: //SendMsg
            split(1,command,user);
            split(2,command,msg);
            sendMsg(user,msg);
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
    if (sockfd < 0){
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

//Encuentra usuario y retorna ID
int findUser(char user[30]){

    for(int i = 0;i<connectionsCounter;i++){
        int large = strlen(connectionList[i].name);
        for (int j = 0; j < large ; j++) {
            if (connectionList[i].name[j] >= 'A' && connectionList[i].name[j] <= 'Z'){
                connectionList[i].name[j] = connectionList[i].name[j] - ('A'-'a');
            }
        }
        large = strlen(user);
        for (int j = 0; j < large ; j++) {
            if (user[j]== '\n' || user[j]== ' ' ){

                user[j]='\0';
            }
        }
        if (strcmp(connectionList[i].name,user)==0){ //se ha encontrado el usuario
            return i;
        }
    }
    return 9999;
}
void sendMsg(char user[30],char data[30]){
    int userID;
    userID = findUser(user);
    if(userID == 9999){
        show("\n User not found!\n");
    }else{
        trama msg;
        trama answer;
        char *headerOk = "[MSGOK]";
        msg.header = malloc(sizeof(char)*strlen("[MSG]"));
        msg.type = 0x02;
        strcpy(msg.header,"[MSG]");
        msg.length = (unsigned short)strlen(data);
        msg.data = malloc(sizeof(char)*msg.length);
        strcpy(msg.data,data);

        write (connectionList[userID].socket, &msg.type, sizeof (char));
        write (connectionList[userID].socket, msg.header, sizeof (char)*strlen(msg.header));
        write (connectionList[userID].socket, &msg.length, sizeof (unsigned short));
        write (connectionList[userID].socket, msg.data, sizeof (char)*msg.length);

        read (connectionList[userID].socket, &answer.type, sizeof (char));
        read (connectionList[userID].socket, answer.header, sizeof (char)*strlen("[MSGOK]"));
        read (connectionList[userID].socket, &answer.length, sizeof (unsigned short));
        answer.data = malloc(sizeof(char)*answer.length);
        read (connectionList[userID].socket, answer.data, sizeof (char)*answer.length);
        if(strcmp(answer.header,headerOk)==0){
            show("Msg Ok");
        }else{
            show("Msg Error");
        }
//        free(headerOk);
//        free(answer.header);
//        free(answer.data);
//        free(msg.header);
//        free(msg.data);
    }
}
void connection(char* puerto,config *Configuration){
    trama msg;
    char buffer[50];
    trama answer;
    sleep(1);
    msg.type = 0x01;
    msg.header = malloc(sizeof(char)*strlen("[TR_NAME]"));
    answer.header = malloc(sizeof(char)*7);
    strcpy(msg.header,"[TR_NAME]");
    char *headerOk = "[CONOK]";
    msg.length = (unsigned short)strlen(Configuration->user);
    msg.data = malloc(sizeof(Configuration->user));
    strcpy(msg.data,Configuration->user);

    int socketTemp = conectionSocket(atoi(puerto)); //Realiza conexion
    //Enviar trama de conexion
    write (socketTemp, &msg.type, sizeof (char));
    write (socketTemp, msg.header, sizeof (char)*strlen(msg.header));
    write (socketTemp, &msg.length, sizeof (unsigned short));
    write (socketTemp, msg.data, sizeof (char)*strlen(Configuration->user));

    //Recibe respuesta
    read (socketTemp, &answer.type, sizeof (char));
    read (socketTemp, answer.header, sizeof (char)*strlen("[CONOK]"));
    read (socketTemp, &answer.length, sizeof (unsigned short));
    answer.data = malloc(sizeof(char)*answer.length);
    read (socketTemp, answer.data, sizeof (char)*answer.length);
    show("Connecting...\n");
    sprintf(buffer,"%s connected: %s \n",puerto, answer.data);
    show(buffer);
    if(strcmp(answer.header,headerOk)==0){
        Connections temp;
        temp.name = malloc(sizeof(char)*strlen(answer.data));
        strcpy(temp.name,answer.data);
        temp.socket = socketTemp;
        connectionList = realloc(connectionList, sizeof(temp));
        connectionList[connectionsCounter].name = malloc(sizeof(char)*strlen(answer.data));
        strcpy(connectionList[connectionsCounter].name,temp.name);
        connectionList[connectionsCounter].socket = temp.socket;
        free(temp.name);
        connectionsCounter++;
    }else{
        show("No ha sido posible realizar conexion");
    }
//    free(headerOk);
//    free(answer.header);
//    free(answer.data);
//    free(msg.header);
//    free(msg.data);
}




