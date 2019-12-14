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
#include "config.h"
#ifndef CYPHERSYSTEM_SYSTEM_H
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
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
//int exitThread = 0;

void read_keyboard(char **string);
void show(char *string);
void terminal();
int findUser(char *user);
int processCommand(char* command);
void exitTrinity();
int analizeCommand(char *command,char **buffer);
void split (int nword, char *text,char **splitted);
void showConnections();
int conectionSocket(int port);
void sendMsg(char *user, char *msg);
void itoa(int num,char* buff);
void connection(char* puerto);
//int sigueActivo(int socket);
int checkPort(int port);
#endif
#endif //CYPHERSYSTEM_SYSTEM_H
