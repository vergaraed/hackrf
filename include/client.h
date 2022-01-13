#ifndef CLIENT_HEADER_FILE
#define CLIENT_HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/client.h"
#include "../include/utils.h"

#define CLIENTPORT    5555
#define MAXMSG  512
#define IP_LEN  10

int clientconnect(char *ip, int port);
//void sendcommand(char * cmd, char* resp);
//int closeconnetion(int sockfd);
//int closeconnetion();

#endif
