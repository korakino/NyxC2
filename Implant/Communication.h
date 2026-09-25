#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <bcrypt.h>
#define DEFAULT_BUFLEN 1024


char *decode(char *message, char key, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        message[i] = message[i] ^ key;
    }
    return message;
}

int recv_all(SOCKET sock, char *buff, int len)
{
    int received = 0;
    int total = 0;
    while (total < len)
    {
        received = recv(sock, buff + total, len - total, 0);
        if (received <= 0)
            return received;
        total += received;
    }
    return total;
}

int send_all(SOCKET sock, char *buff, int len)
{
    int sent = 0;
    int total = 0;
    while (total < len)
    {
        sent = send(sock, buff + total, len - total, 0);
        if (sent <= 0)
            return sent;
        total += sent;
    }
    return total;
}