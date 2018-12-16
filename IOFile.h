#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef PROGETTOALGORITMI_IOFILE_H
#define PROGETTOALGORITMI_IOFILE_H

FILE* openFile(FILE *fp, char *nameFile, bool compressName);
FILE* readFile(FILE *fp, char *nameFile);
void writeByte(FILE *fp, unsigned char codedStr);
void closeFile(FILE *fp);

#endif PROGETTOALGORITMI_IOFILE_H
