//
// Created by Simone on 07.12.18.
//

#include "IOFile.h"

/*
 * se writable è settato a true, allora il file viene aperto in modalita write
 */
FILE* openFile(FILE *fp, char *nameFile, bool writeable){
    return writeable?fopen(nameFile, "wb"):fopen(nameFile, "rb");
}

/*
 * funzione per scrivere sul file aperto e passato come parametro
 */
void writeByte(FILE *fp, unsigned char codedStr){
    fputc(codedStr, fp);
}

/*
 * funzione che chiude il file aperto
 */
void closeFile(FILE *fp){
    fclose(fp);
}
