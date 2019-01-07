//
// Created by attilio on 03/12/18.
//

#include "utils.h"

char buffer[26];

char * timeStamp(){
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
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