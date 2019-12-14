#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include "system.h"
#include "config.h"

#ifndef CYPHERSYSTEM_SERVER_H
#define CYPHERSYSTEM_SERVER_H


void *createServer();
void checkTrams(char type,int socketTemp,char **username);

#endif //CYPHERSYSTEM_SERVER_H
