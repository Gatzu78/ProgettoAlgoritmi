//
// Created by Simone on 07.12.18.
//

#include "IOFile.h"

/*
 * compressName if is setted true than will be added the compress name, otherwise the real file name
 */
FILE* openFile(FILE *fp, char *nameFile, bool compressName){
    if(fp == NULL){
        char *type = ".funn";
        char *name = malloc (strlen(nameFile) + strlen(type) + 1);;
        strcat(name, nameFile);
        strcat(name, type);
        return fopen(name, "wb");
    }
    return  NULL;
}

FILE* readFile(FILE *fp, char *nameFile){
    if(fp == NULL){
        char *type = ".funn";
        char *name = malloc (strlen(nameFile) + strlen(type) + 1);;
        strcat(name, nameFile);
        strcat(name, type);
        return fopen(name, "rb");
    }
    return  NULL;
}


void writeByte(FILE *fp, unsigned char codedStr){
    fputc(codedStr, fp);
}

void closeFile(FILE *fp){
    fclose(fp);
}
