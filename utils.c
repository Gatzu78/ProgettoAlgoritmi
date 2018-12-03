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