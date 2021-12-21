#ifndef UTILS_HEADER_FILE
#define UTILS_HEADER_FILE
//#include "../../include/utils.h"
#include "utils.h"

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

bool parseClientInput( const char * deviceName, char *commandOut, const int buffsize )
{
    // Wait for the user to press <enter> on the console to quit the application
    printf("Enter quit, bye, close or exit to quit and close client connection.\n");

    int rc;
    //char command[50];
    //char *command;
    //command = malloc(buffsize);
    char *cmdPrompt = malloc(buffsize);
    snprintf(cmdPrompt, buffsize, "Command for %s\n", deviceName);
    
    rc = getLine (cmdPrompt, commandOut, buffsize);
        
    if (rc == NO_INPUT) 
    {
        // Extra NL since my system doesn't output that on EOF.
        printf ("\nNo input\n");
    }
    else if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", commandOut);
    } 
    else 
    {
        printf ("Input [%s]\n", commandOut);

        if((strcmp(commandOut, "exit") == 0 || strcmp(commandOut, "quit") ==0 ||
                    strcmp(commandOut, "quit") ==0) || strcmp(commandOut, "close") == 0 )
        {
            printf("Closing client socket connection..\n");
            snprintf(commandOut, buffsize,"%s", commandOut);
            return true;
        }
    
        printf ("OK [%s]\n", commandOut);
        if((strcmp(commandOut, "help") == 0 || strcmp(commandOut, "h") ==0 || strcmp(commandOut, "?") ==0))
        {
            printf("HackRF Help Menu\n\n");
            printf("\t--------\n");
            printf("\tCommands\n");
            printf("\t--------\n");
            printf("\tf - Get frequency [Hz]\n");
            printf("\tF - Set frequency [Hz]\n");
            printf("\tm - Get demodulator mode\n");
            printf("\tM - Set demodulator mode (OFF, RAW, AM, FM, WFM, WFM_ST,\n");
            printf("\t\t   WFM_ST_OIRT, LSB, USB, CW, CWL, CWU\n");
            printf("\tSTRENGTH - Get signal strength [dBFS]\n");
            printf("\tSQL - Get squelch threshold [dBFS]\n");
            printf("\tSQL <sql> - Set squelch threshold to <sql> [dBFS]\n");
            printf("\tu RECORD - Get status of audio recorder\n");
            printf("\tU RECORD <status> - Set status of audio recorder to <status>\n");
            printf("\tc - Close connection\n");
            printf("\tAOS - Acquisition of signal (AOS) event, start audio recording\n");
            printf("\tLOS - Loss of signal (LOS) event, stop audio recording\n");
            printf("\tdump_state - Dump state (only usable for compatibility)\n");
            printf("\t\texit, quit, bye or x:\tShutdown and Quit CryptReserve Node\n");
            printf("\t\thelp, h or ?:\tCryptReserve Help Menu.\n");
            return true;
        }

    }

 
   return false;
}

bool parseUserInput()
{
    // Wait for the user to press <enter> on the console to quit the application
    printf("Enter quit, bye or exit to quit server\n");

    int rc;
    //char command[50];
    char *command;
    command = malloc(65);
    rc = getLine ("Enter command> ", command, 65);
    if (rc ==0)
        return true;

    if (rc == NO_INPUT) {
    // Extra NL since my system doesn't output that on EOF.
        printf ("\nNo input\n");
    }

    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", command);
    }

    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", command);
    }

    printf ("Input [%s]\n", command);

    if(strncmp(command, "connect", strlen("connect")) == 0 ||
            strncmp(command, "c", 1) ==0 )
    {
        clientconnect("127.0.0.1", 5555);

        return 1;

        char *cmd;
        char *ip;
        char *port;

      printf("raw cmd:\t%s\n", command);
        cmd = strtok(command, " ");
        //printf("cmd:\t%s\n", tok);
        if(cmd != NULL)
        {   
            ip = strtok(NULL, " ");
            printf("ip:\t%s\n", ip);
            if(ip != NULL)
            {   
                port = strtok(NULL, " ");
                printf("port:\t%d\n", port);
                if(port != NULL)
                {   
                    printf("User requested connect\n\tcommand=%s\n\tip=%s\n\tport=%s\n", cmd,ip,port);
                    clientconnect(ip, atoi((const char*)port));
                }
            }
        }
    }

    if((strcmp(command, "exit") == 0 || strcmp(command, "quit") ==0 || strcmp(command, "quit") ==0))
    {
        printf("User requested quitting.\n");
        return 0;
    }

    printf ("OK [%s]\n", command);
    if((strcmp(command, "help") == 0 || strcmp(command, "h") ==0 || strcmp(command, "?") ==0))
    {
        printf("CryptReserve Help Menu\n\n");
        printf("\tCommands\n");
        printf("\t--------\n");
        printf("\t\texit, quit, bye or x:\tShutdown and Quit CryptReserve Node\n");
        printf("\t\thelp, h or ?:\tCryptReserve Help Menu.\n");;
        return 1;
    }

    return false;
}

static int getLine (char *prmpt, char *buff, size_t sz) 
{
    int ch, extra;

    printf ("getLine %d\n",sz);
    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

#endif
