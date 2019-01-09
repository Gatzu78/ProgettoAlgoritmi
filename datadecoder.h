//
// Created by attilio on 07/01/19.
//

#ifndef PROGETTOALGORITMI_DATADECODER_H
#define PROGETTOALGORITMI_DATADECODER_H


#include <stdio.h>
#define BLOCKSIZE 8

extern int decoderWriteSelector;

/* formatter usato per preparare il dato da scrivere */
typedef struct decoder{
    unsigned int count;
    //unsigned char bitIndex;
    unsigned short istructionDecoder[BLOCKSIZE];
    unsigned int bitCoded[BLOCKSIZE]; // per l'implementazione 2048 - 32, 16 bit, basterebbe un unsigned short
}decoder;

decoder * newDecoder();
int deleteDecoder(decoder * currentDecoder);
decoder * addToDecoder(decoder * currentDecoder, unsigned int offset, unsigned char posRight, FILE *fileptr);
int writeDecoder(decoder * currentDecoder, FILE *fileptr);
unsigned char *  writeDecoderToBuffer(decoder * currentDecoder);
int writeDecoderToFile(decoder * currentDecoder, FILE *fileptr);
#endif //PROGETTOALGORITMI_DATADECODER_H
