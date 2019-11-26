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

int main(int argc, char *argv[]) {
    if(argc!=2) {
        show(" Put filename \n");
        exit(1);
    }
    config *Configuration;
    Configuration = (config*) malloc(sizeof(config));
    processConfig(Configuration,argv[1]);
    createServer(Configuration->port);
    //------Terminal----------
    terminal(Configuration);
    free(Configuration);
    return 0;
}