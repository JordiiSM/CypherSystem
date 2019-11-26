#include "config.h"
#ifndef CYPHERSYSTEM_SYSTEM_H
#define CYPHERSYSTEM_SYSTEM_H

#endif //CYPHERSYSTEM_SYSTEM_H

void read_keyboard(char *string);
void show(char *string);
void terminal(config *Configuration);
int processCommand(char* command,config *Configuration);
int analizeCommand(char command[50]);
void split (int nword, char text[50],char* splitted);
void showConnections(config *Configuration);
int conectionSocket(int port);
void sendMsg(int socket, char msg[30]);
void itoa(int num,char* buff);