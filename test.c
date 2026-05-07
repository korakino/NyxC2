#include "implant.h"
char* decode(char* message, char key, int lenght){
    for (int i = 0; i < lenght; i++){
        message[i] = message[i] ^ key;
    }
    return message;
}
int main(){
    char* message = "";
    decode(message, 'K', strlen(message))
    printf("%s\n", message);





}