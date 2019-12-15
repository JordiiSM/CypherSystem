#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#ifndef CYPHERSYSTEM_CONFIG_H
#define CYPHERSYSTEM_CONFIG_H

typedef struct {

    char *user;
    char *folder;
    char *ip;
    int port;
    char *connectionIP;
    int connectionPortIni;
    int connectionPortFin;

}config;
config Configuration;
int exitThread;
void processConfig (char *filename);
void read_word_text(char **string, int fd, char hasta);

#endif //CYPHERSYSTEM_CONFIG_H
