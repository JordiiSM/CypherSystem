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


#ifndef CYPHERSYSTEM_SERVER_H
#define CYPHERSYSTEM_SERVER_H

void createServer(int port);
trama checkTrams(trama t);

#endif //CYPHERSYSTEM_SERVER_H
