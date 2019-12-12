#include "config.h"
#ifndef CYPHERSYSTEM_SYSTEM_H
#define CYPHERSYSTEM_SYSTEM_H


typedef struct {

    char type;
    char *header;
    unsigned short length;
    char *data;

} trama;

typedef struct {

    char *name;
    int socket;
    int port;

} Connections;




void read_keyboard(char *string);
void show(char *string);
void terminal();
int findUser(char user[30]);
int processCommand(char* command);
void exitTrinity();
int analizeCommand(char command[50]);
void split (int nword, char text[50],char* splitted);
void showConnections();
int conectionSocket(int port);
void sendMsg(char user[30], char msg[30]);
void itoa(int num,char* buff);
void connection(char* puerto);
int sigueActivo(int socket);
int checkPort(int port);

#endif //CYPHERSYSTEM_SYSTEM_H
