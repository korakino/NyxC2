#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> //for windows computers
#include <windows.h>
#define DEFAULT_BUFLEN 1024


char* decode(char* message, char key, int lenght);