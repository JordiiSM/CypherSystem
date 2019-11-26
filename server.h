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

#ifndef CYPHERSYSTEM_SERVER_H
#define CYPHERSYSTEM_SERVER_H

void createServer(int port);

#endif //CYPHERSYSTEM_SERVER_H
