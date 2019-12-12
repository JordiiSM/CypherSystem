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
//    char *buffer = NULL;
    char caracter = ' ';

    while( caracter != hasta) {

        if (read(fd, &caracter, sizeof(char)) == 0) {

        } else {

            string = (char *) realloc(string, sizeof(char)* (i + 1));

            string[i] = caracter;

            if (caracter == hasta) {
                string[i] = '\0';
            }

            i++;
        }
    }
}
void processConfig (char *filename){
    int fd;
    fd = open(filename,O_RDWR | O_APPEND | O_CREAT);
    printf("checkpoint2\n");

    char *tempport = malloc(sizeof(char));
    char *tempport2 = malloc(sizeof(char));
    char *tempport3 = malloc(sizeof(char));

    Configuration.user = malloc(sizeof(char));
    Configuration.folder = malloc(sizeof(char));
    Configuration.ip = malloc(sizeof(char));
    Configuration.connectionIP = malloc(sizeof(char));
    read_word_text(Configuration.user, fd, '\n');
    read_word_text(Configuration.folder, fd, '\n');
    read_word_text(Configuration.ip, fd, '\n');
    read_word_text(tempport, fd, '\n');
    Configuration.port = atoi(tempport);
    read_word_text(Configuration.connectionIP, fd, '\n');
    read_word_text(tempport2, fd, '\n');
    Configuration.connectionPortIni = atoi(tempport2);
    read_word_text(tempport3, fd, '\n');
    Configuration.connectionPortFin = atoi(tempport3);

    free(tempport);
    free(tempport2);
    free(tempport3);
    close(fd);

}