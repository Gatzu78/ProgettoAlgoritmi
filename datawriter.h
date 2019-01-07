//
// Created by attilio on 17/12/18.
//

#ifndef PROGETTOALGORITMI_DATAWRITER_H
#define PROGETTOALGORITMI_DATAWRITER_H

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
formatter * addToFormatter(formatter * currentFormatter, unsigned int offset, unsigned char posRight, char * outputPath);
int writeFormatter(formatter * currentFormatter, char * outputPath);
unsigned char *  writeToBuffer(formatter * currentFormatter);
int writeToFile(formatter * currentFormatter, char * outputPath);


#endif //PROGETTOALGORITMI_DATAWRITER_H
