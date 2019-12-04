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

} Connections;


void read_keyboard(char *string);
void show(char *string);
void terminal(config *Configuration);
int findUser(char user[30]);
int processCommand(char* command,config *Configuration);
int analizeCommand(char command[50]);
void split (int nword, char text[50],char* splitted);
void showConnections(config *Configuration);
int conectionSocket(int port);
void sendMsg(char user[30], char msg[30]);
void itoa(int num,char* buff);
void connection(char* puerto, config *Configuration);

#endif //CYPHERSYSTEM_SYSTEM_H
