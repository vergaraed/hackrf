#include <stdio.h>
#include <stdbool.h>

#define NO_INPUT 1
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

char* concat(const char *s1, const char *s2);
static int getLine(char *prmpt, char *buff, size_t sz);
bool parseClientInput( const char * deviceName, char *commandOut, const int buffsize );
bool parseUserInput();

