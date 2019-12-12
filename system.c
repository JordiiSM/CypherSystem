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
//---------------------------------------TERMINAL----------------------------------------
void terminal(){
    char buffer[100];
    memset(buffer,'\0',100);
    char command[50];
    int option = 0;
    while (option != 1) {
        option = 0;
        sprintf(buffer, "\n\033[1;31m $%s \033[0m", Configuration.user);
        show(buffer);
        read_keyboard(command);
        option = processCommand(command);
    }
    show(buffer);
    show(" \nBye!!\n");
}
//-------------Procesar comandos y llamar a la funcion de cada comando------------
int processCommand(char command[50]){
    int option = 0;
    char* puerto = malloc(50*sizeof(char));
    char* user = malloc(50*sizeof(char));
    char* msg = malloc(500* sizeof(char));
    int portCheck = 0;
    option = analizeCommand(command);
    switch (option){
        case 0:
            show("Command not recognized");
            break;
        case 1:
            showConnections();
            break;
        case 2: //Connect
            split(1,command,puerto);
            portCheck = checkPort(atoi(puerto));
            if(portCheck == 1){
                connection(puerto);
                portCheck = 0;
            }
            break;
        case 3: //SendMsg
            split(1,command,user);
            split(2,command,msg);
            sendMsg(user,msg);
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
            exitTrinity();
            return 1;
    }
    free(puerto);
    free(user);
    return 0;
}
//---------------Analizar comandos y retornar que comando se ha escrito-------------------
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
// Control errores puerto (Si es tu puerto o si ya estas conectado a este puerto)
int checkPort(int port){
    if(port == Configuration.port){
        show("No te puedes conectar a ti mismo\n");
        return 0;
    }else{
        for(int i = 0;i<connectionsCounter;i++){
            if (port == connectionList[i].port){
                show("Ya estas conectado a este puerto");
                return 0;
            }
        }
    }
    return 1;
}
//------------------Funcion para separar palabras por espacio-------------
 void split (int nword, char text[50],char* splitted){
    char nsplit = 0;
    int j = 0;
    for(int i=0;i<50;i++){
        if(nsplit == nword || (nword ==2 && nsplit > 2)){
            splitted[j] = text[i];
            j++;
        }
        if(text[i] == ' ' && text[i+1] != ' '){
            nsplit++;
        }
    }
}
//----------------------EJECUTA SCRIPT PARA VER CONEXIONES DISPONIBLES-----------------
void showConnections(){
    int connectionFork;
    char buffer[100];
    connectionFork = fork();

    if (connectionFork > 0) { //Padre
        sleep(1);
    }
    if (connectionFork == 0) { //Hijo
        char *portini = malloc(sizeof(char)*4);
        char *portfin = malloc(sizeof(char)*4);
        itoa(Configuration.connectionPortIni,portini);
        itoa(Configuration.connectionPortFin,portfin);
        sprintf(buffer, "\n\033[0;33mAVAILABLE CONNECTIONS %s / %s \n\033[0m",portini,portfin);
        show(buffer);
        char *argv[4];
        argv[0] = "./show_connections.sh";
        argv[1] = portini;
        argv[2] = portfin;
        argv[3] = NULL;
        execvp("show_connections.sh",argv);
//        execl("./show_connections.sh",portini,portfin,(char *)0);
//        char* arr[] = {"./show_connections.sh", portini, portfin, NULL};
//        execv("./show_connections", arr);
    }
}
//-------------INT TO ASCII---------------------
void itoa(int num,char* buff){
    sprintf(buff,"%d",num);
}
//---------Prueba conexion socket------------------------------
int sigueActivo(int socket){
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &len);
    if (retval != 0) {
        /* there was a problem getting the error code */
        fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        return 1;
    }

    if (error != 0) {
        /* socket has a non zero error status */
        fprintf(stderr, "socket error: %s\n", strerror(error));
        return 0;
    }
}
//-------------------Conecta socket-----------------------------
int conectionSocket(int port){

    struct in_addr ip_addr;
    if (inet_aton ("127.0.0.1", &ip_addr) == 0){ //ip
        fprintf (stderr, "inet_aton (%s): %s\n", "127.0.0.1", strerror (errno)); //ip
    }

    int sockfd;
    sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0){
        perror ("socket TCP");
    }
    struct sockaddr_in s_addr;
    bzero (&s_addr, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (port);
    s_addr.sin_addr = ip_addr;

    if (connect (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0)
    {
        perror ("connect"); //peta
        return 0;
    }
    return sockfd;
}

//---------------Encuentra usuario y retorna ID--------------------------
int findUser(char user[30]){

    for(int i = 0;i<connectionsCounter;i++){    //Recorre todas las conexiones guardadas
        int large = strlen(connectionList[i].name);
        for (int j = 0; j < large ; j++) { //Cambia mayusculas a minusculas para ser case insensitive
            if (connectionList[i].name[j] >= 'A' && connectionList[i].name[j] <= 'Z'){
                connectionList[i].name[j] = connectionList[i].name[j] - ('A'-'a');
            }
        }
        large = strlen(user);
        for (int j = 0; j < large ; j++) { //Reemplaza caracteres inutiles por \0
            if (user[j]== '\n' || user[j]== ' ' ){

                user[j]='\0';
            }
        }
        if (strcmp(connectionList[i].name,user)==0){ //se ha encontrado el usuario
            return i;   //devuelve la posicion del usuario a conectarse (Para asi saver el socket)
        }
    }
    return 9999;    //Error (usuario no encontrado)
}
//----------------Envia mensajes a servidor y lee respuesta-------------------------
void sendMsg(char user[30],char data[30]){
    int userID;
    userID = findUser(user);
    if(userID == 9999){ //Usuario no encontrado
        show("\n User not found!\n");
    }else {
        trama msg; //trama que se envia al servidor
        trama answer;   //trama que devuelve el servidor
//        char *headerOk = "[MSGOK]";
        msg.header = malloc(sizeof(char) * strlen("[MSG]"));
        msg.type = 0x02;
        strcpy(msg.header, "[MSG]");
        msg.length = (unsigned short) strlen(data);
        msg.data = malloc(sizeof(char) * msg.length);
        strcpy(msg.data, data);
        int checkSocket = sigueActivo(connectionList[userID].socket);
//        if(checkSocket==1){
        //Envia mensaje
        write(connectionList[userID].socket, &msg.type, sizeof(char)) ;
        write(connectionList[userID].socket, msg.header, sizeof(char) * strlen(msg.header));
        write(connectionList[userID].socket, &msg.length, sizeof(unsigned short));
        write(connectionList[userID].socket, msg.data, sizeof(char) * msg.length);
        //Recive respuesta
        read(connectionList[userID].socket, &answer.type, sizeof(char));
        read(connectionList[userID].socket, answer.header, sizeof(char) * strlen("[MSGOK]"));
        read(connectionList[userID].socket, &answer.length, sizeof(unsigned short));
        answer.data = malloc(sizeof(char) * answer.length);
        read(connectionList[userID].socket, answer.data, sizeof(char) * answer.length);
//        }else{
//            printf("Socket caido\n");
//        }
//        if(strcmp(answer.header,headerOk)==0){ //Mensaje enviado y procesado por servidor correctamente
//            show("Msg Ok");
//        }else{  //Error en el servidor al procesar trama
//            show("Msg Error");
//        }
//        free(headerOk);
//        free(answer.header);
//        free(answer.data);
//        free(msg.header);
//        free(msg.data);


    }
}
void connection(char* puerto){
    trama msg;
    char buffer[50];
    trama answer;
    sleep(1);
    msg.type = 0x01;
    msg.header = malloc(sizeof(char)*strlen("[TR_NAME]"));
    answer.header = malloc(sizeof(char)*7);
    strcpy(msg.header,"[TR_NAME]");
    char *headerOk = "[CONOK]";
    msg.length = (unsigned short)strlen(Configuration.user);
    msg.data = malloc(sizeof(Configuration.user));
    strcpy(msg.data,Configuration.user);

    int socketTemp = conectionSocket(atoi(puerto)); //Realiza conexion
    if(socketTemp != 0){
        //envia trama
        write (socketTemp, &msg.type, sizeof (char));
        write(socketTemp, msg.header, sizeof(char) * strlen(msg.header));
        write(socketTemp, &msg.length, sizeof(unsigned short));
        write(socketTemp, msg.data, sizeof(char) * strlen(Configuration.user));

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
        connectionList[connectionsCounter].port = atoi(puerto);
        free(temp.name);
        connectionsCounter++;
    }else{
        show("No ha sido posible realizar conexion");
    }
    }
//    free(headerOk);
//    free(answer.header);
//    free(answer.data);
//    free(msg.header);
//    free(msg.data);
}
void exitTrinity(){
    trama bye;
    sleep(1);
    bye.type = 0x06;
    bye.header = malloc(sizeof(char)*strlen("[]"));
    strcpy(bye.header,"[]");
    bye.length = (unsigned short)strlen(Configuration.user);
    bye.data = malloc(sizeof(Configuration.user));
    strcpy(bye.data,Configuration.user);
    for(int i = 0;i<connectionsCounter;i++){
        write (connectionList[i].socket, &bye.type, sizeof (char));
        write (connectionList[i].socket, bye.header, sizeof (char)*strlen(bye.header));
        write (connectionList[i].socket, &bye.length, sizeof (unsigned short));
        write (connectionList[i].socket, bye.data, sizeof (char)*bye.length);
    }
    exit(1);

}




