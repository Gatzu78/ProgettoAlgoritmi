//
// Created by attilio on 07/01/19.
//
#include <stdlib.h>
#include "utils.h"
#include "datadecoder.h"

decoder * newDecoder(){
    decoder * currenDecoder=(decoder*)malloc(sizeof(decoder));
    currenDecoder->count=0;
    int i=0;
    for(i;i<BLOCKSIZE;i++){
        currenDecoder->istructionDecoder[i]=0;
        currenDecoder->bitCoded[i]=0;
    }
    return currenDecoder;
}
int deleteDecoder(decoder * currentDecoder){
    free(currentDecoder);
}
decoder * addToDecoder(decoder * currentDecoder, unsigned int offset, unsigned char posRight, FILE *fileptr);
int writeDecoder(decoder * currentDecoder, FILE *fileptr){
    int i = 0;
    for(i;i<8;i++){
        if(currentDecoder->istructionDecoder[i]==0){
        #ifdef DEBUGOUTPUT
            printf("%c",currentDecoder->bitCoded[i]);
        #endif
            printf("Prima della scrittura %ld\n", ftell(fileptr));
            fwrite(&currentDecoder->bitCoded[i],sizeof(unsigned char),1,fileptr);
            fflush(fileptr);
        } else{
            unsigned int numChars=currentDecoder->bitCoded[i] & 0x1F;
            long jump=currentDecoder->bitCoded[i] & 0xFFE0;
            jump=jump>>5;
            unsigned char buffer[numChars];
            fseek(fileptr,-jump,SEEK_END);
            printf("Prima della lettura %ld\n", ftell(fileptr));
            fread(&buffer,sizeof(unsigned char),numChars,fileptr);
            fflush(fileptr);
            #ifdef DEBUGOUTPUT
                int j=0;
                for(j;j<numChars;j++) {
                printf("%c", buffer[j]);
                }
            #endif
            fseek(fileptr,0,SEEK_END);
            printf("Prima della scrittura %ld\n", ftell(fileptr));
            fwrite(&buffer,sizeof(unsigned char),numChars,fileptr);
            fflush(fileptr);
            }
        }

    return 0;
}


unsigned char *  writeDecoderToBuffer(decoder * currentDecoder);
int writeDecoderToFile(decoder * currentDecoder, FILE *fileptr);
