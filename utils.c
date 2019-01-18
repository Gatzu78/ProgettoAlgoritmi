//
// Created by attilio on 03/12/18.
//

#include "utils.h"

char timeStringBuffer[26];

char * timeStamp(){
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(timeStringBuffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return timeStringBuffer;
}

void showbits(unsigned int x, short numExa) {
    for(int i = (sizeof(int) * numExa) - 1; i >= 0; i--) {
        (x & (1u << i)) ? putchar('1') : putchar('0');
    }
    printf("\n");
}

void emptyString(char * str, int size){
    for (size;size>0;size--){
        str[(size-1)]='\0';
    }
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 per il terminatore /0
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}