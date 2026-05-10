// #include "implant.h"
#include <stdio.h>
#include <string.h>

char* decode(char* message, char key, int lenght);
void show(char* message, int lenght);



int main(){
    char message[] = "WSAStartup";
    unsigned char encoded[] = {0x20, 0x2E, 0x39, 0x25, 0x2E, 0x27, 0x78};
    int size = sizeof(message);
    printf("%d\n", size);
    decode(message, 'K', size - 1);
    show(message, size - 1);




    return 0;
}
char* decode(char* message, char key, int lenght){
    for (int i = 0; i < lenght; i++){
        message[i] = message[i] ^ key;
    }
    return message;
}

void show(char* message, int lenght){
    printf("{");
    for (int i = 0; i < lenght; i++){
        printf("0x%02X, ", (unsigned char)message[i]);
    }

    printf("0x00}\n");

}