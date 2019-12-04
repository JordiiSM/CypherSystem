#ifndef CYPHERSYSTEM_CONFIG_H
#define CYPHERSYSTEM_CONFIG_H

char *username;

typedef struct {

    char user[50];
    char folder[10];
    char ip[15];
    int port;
    char connectionIP[50];
    int connectionPortIni;
    int connectionPortFin;

}config;


void processConfig (config *configuration, char *filename);
void read_word_text(char *string, int fd, char hasta);

#endif //CYPHERSYSTEM_CONFIG_H
