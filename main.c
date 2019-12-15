/////////////////////////////////////////////
/////////-----JORDI SABANES------///////////
////////-------JOAN CUSCO------////////////
//////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "config.h"
#include "server.h"
#include <signal.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    exitThread = 0;
    pthread_t threadServer;
    if(argc!=2) {
        show(" Put filename \n");
        exit(1);
    }
    signal (SIGINT, exitTrinity);
    processConfig(argv[1]);
    pthread_create(&threadServer, NULL, createServer, NULL);
    //------Terminal----------
    terminal();

    return 0;
}