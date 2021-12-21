#ifndef CLIENT_HEADER_FILE
#define CLIENT_HEADER_FILE
#include <stdio.h>
#include <stdlib.h>

#define CLIENTPORT    5555
#define MAXMSG  512
#define IP_LEN  10

typedef struct
{

} hackrf_sweep_args;

//int clientconnect(char *ip,int port);
int clientconnect(void (*_fft_power_callback)(char /*full_sweep_done*/), char *ip, int port);
//void sendcommand(char * cmd, char* resp);
//int closeconnetion(int sockfd);
//int closeconnetion();

#endif
