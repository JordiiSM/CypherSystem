//
// Created by Jordi on 12/11/2019.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"



void read_word_text(char *string, int fd, char hasta){
    int i = 0;
    char *buffer = NULL;
    char caracter = ' ';

    while( caracter != hasta) {

        if (read(fd, &caracter, sizeof(char)) == 0) {

        } else {

            buffer = (char *) realloc(buffer, i + 1);

            buffer[i] = caracter;

            if (caracter == hasta) {
                buffer[i] = '\0';
            }

            i++;
        }
    }
    strcpy(string,buffer);
    free(buffer);
}
void processConfig (config *configuration, char *filename){
    int fd;
    fd = open(filename,O_RDWR | O_APPEND | O_CREAT);
    char tempport[5];
    int i = 0;
        read_word_text(configuration->user, fd, '\n');
        username = malloc(sizeof(char)*strlen(configuration->user));
        strcpy(username,configuration->user);
        read_word_text(configuration->folder, fd, '\n');
        read_word_text(configuration->ip, fd, '\n');
        read_word_text(tempport, fd, '\n');
        configuration->port = atoi(tempport);
        read_word_text(configuration->connectionIP, fd, '\n');
        read_word_text(tempport, fd, '\n');
        configuration->connectionPortIni = atoi(tempport);
        read_word_text(tempport, fd, '\n');
        configuration->connectionPortFin = atoi(tempport);
        i++;

    close(fd);

}