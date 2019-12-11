/////////////////////////////////////////////
/////////-----JORDI SABANES------///////////
////////-------JOAN CUSCO------////////////
/////////////////////////////////////////
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
    pthread_t threadServer;
    if(argc!=2) {
        show(" Put filename \n");
        exit(1);
    }
    signal (SIGINT, exitTrinity);
    Configuration = (config*) malloc(sizeof(config));
    processConfig(Configuration,argv[1]);
    pthread_create(&threadServer, NULL, createServer, NULL);
    //------Terminal----------
    terminal(Configuration);
    free(Configuration);
    return 0;
}