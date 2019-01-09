//
// Created by attilio on 17/12/18.
//

#ifndef PROGETTOALGORITMI_DATAWRITER_H
#define PROGETTOALGORITMI_DATAWRITER_H

#include <stdio.h>

#define BLOCKSIZE 8

extern int formatterWriteSelector;

/* formatter usato per preparare il dato da scrivere */
typedef struct formatter{
    unsigned int count;
    unsigned char bitIndex;
    unsigned short istructionCoder[BLOCKSIZE];
    unsigned int bitCoded[BLOCKSIZE]; // per l'implementazione 2048 - 32, 16 bit, basterebbe un unsigned short
}formatter;

formatter * newFormatter();
int deleteFormatter(formatter * currentFormatter);
formatter * addToFormatter(formatter *currentFormatter, unsigned int offset, unsigned char posRight, FILE *fileptr);
int writeFormatter(formatter *currentFormatter, FILE *fileptr);
unsigned char *  writeToBuffer(formatter * currentFormatter);
int writeToFile(formatter *currentFormatter, FILE *fileptr);


#endif //PROGETTOALGORITMI_DATAWRITER_H
