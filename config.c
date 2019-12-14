//
// Created by Jordi on 12/11/2019.
//

#include "config.h"



void read_word_text(char **string, int fd, char hasta){
    int i = 0;
//    char *buffer = NULL;
    char caracter = ' ';

    while( caracter != hasta) {

        if (read(fd, &caracter, sizeof(char)) == 0) {

        } else {
            *string = (char *) realloc(*string, sizeof(char)* (i + 1));
            (*string)[i] = caracter;

            if (caracter == hasta) {
                (*string)[i] = '\0';
            }

            i++;
        }
    }
}
void processConfig (char *filename){
    int fd;
    fd = open(filename,O_RDWR | O_APPEND | O_CREAT);
    char *tempport = NULL;
    Configuration.user = NULL;
    Configuration.folder = NULL;
    Configuration.ip = NULL;
    Configuration.connectionIP = NULL;

    read_word_text(&Configuration.user, fd, '\n');
    read_word_text(&Configuration.folder, fd, '\n');
    read_word_text(&Configuration.ip, fd, '\n');
    read_word_text(&tempport, fd, '\n');

    Configuration.port = atoi(tempport);
    free(tempport);
    tempport = NULL;

    read_word_text(&Configuration.connectionIP, fd, '\n');
    read_word_text(&tempport, fd, '\n');
    Configuration.connectionPortIni = atoi(tempport);
    free(tempport);
    tempport = NULL;
    read_word_text(&tempport, fd, '\n');
    Configuration.connectionPortFin = atoi(tempport);
    free(tempport);

    close(fd);

}