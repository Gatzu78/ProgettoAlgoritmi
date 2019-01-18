//
// Created by Simone on 07.12.18.
//

#include "IOFile.h"

/*
 * writeable if is setted true than will open in write mode
 */
FILE* openFile(FILE *fp, char *nameFile, bool writeable){
    return writeable?fopen(nameFile, "wb"):fopen(nameFile, "rb");
}

void writeByte(FILE *fp, unsigned char codedStr){
    fputc(codedStr, fp);
}

void closeFile(FILE *fp){
    fclose(fp);
}
