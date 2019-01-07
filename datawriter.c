//
// Created by attilio on 17/12/18.
//
#include <stdlib.h>
#include <stdbool.h>
#include "datawriter.h"
#include "utils.h"

int firstWrite=true;
int formatterWriteSelector = 1;


formatter * newFormatter(){
    formatter * currentFormatter=(formatter*)malloc(sizeof(formatter));
    currentFormatter->count=0;
    currentFormatter->bitIndex=0;
    int i=0;
    for(i;i<BLOCKSIZE;i++){
        currentFormatter->bitCoded[i]=0;
    }
    return currentFormatter;
}

int deleteFormatter(formatter * currentFormatter){
    free(currentFormatter);
};

formatter * addToFormatter(formatter * currentFormatter, unsigned int offset, unsigned char data, char * outputPath){
    if(currentFormatter->count>7){
        writeFormatter(currentFormatter,outputPath);
        deleteFormatter(currentFormatter);
        currentFormatter=newFormatter();
    }
    #ifdef DEBUG
        printf("currentFormatter->count vale %d\n",currentFormatter->count);
        if (offset==0){
            printf("dato non compressa di valore -> '%c'(%u)\n",data,(unsigned int)data);
        }else{
            printf("dato compresso di valore offset relativo %d e spostamento %d\n",offset,(unsigned int)data);
        }
    #endif
    if(offset==0){
        currentFormatter->istructionCoder[currentFormatter->count]=0;
        currentFormatter->bitIndex=currentFormatter->bitIndex<<1;
        currentFormatter->bitIndex+=0;
        currentFormatter->bitCoded[currentFormatter->count]=data;
        currentFormatter->count++;
        #ifdef DEBUG
            showbits(currentFormatter->bitCoded[currentFormatter->count]); // stampa la codifica
        #endif

    }else{
        currentFormatter->istructionCoder[currentFormatter->count]=1;
        currentFormatter->bitIndex=currentFormatter->bitIndex<<1;
        currentFormatter->bitIndex+=1;
        currentFormatter->bitCoded[currentFormatter->count]=offset;
        currentFormatter->bitCoded[currentFormatter->count]=currentFormatter->bitCoded[currentFormatter->count]<<5;
        currentFormatter->bitCoded[currentFormatter->count]+=(unsigned int)data;
        currentFormatter->count++;
        #ifdef DEBUG
            showbits(currentFormatter->bitCoded[currentFormatter->count]); // stampa la codifica
        #endif
    }
    return currentFormatter;
};

int writeFormatter(formatter * currentFormatter, char * outputPath){

    if(formatterWriteSelector==0){
        writeToBuffer(currentFormatter);
    } else {
        writeToFile(currentFormatter,outputPath);
    }
};
unsigned char * writeToBuffer(formatter * currentFormatter){
    return "0";
    //non implementato, di fondo servirebbe a passare via puntatote a memoria la codifica.
}
int writeToFile(formatter * currentFormatter, char * outputPath){
    #ifdef DEBUGOUTPUT
        printf("Indice = ");
        showbits(currentFormatter->bitIndex,2);
    #endif
    FILE *fileptr;
    if (firstWrite==true){
        fileptr = fopen(outputPath, "wb"); //sovrascrive se il file esiste
        firstWrite=false;
    } else {
        fileptr = fopen(outputPath, "ab"); //appende i dati al file esistente
    }
    fwrite(&currentFormatter->bitIndex,1,sizeof(unsigned char),fileptr);
    int i=0;
    for(i;i<BLOCKSIZE;i++){
        if(currentFormatter->istructionCoder[i]==0){
            #ifdef DEBUGOUTPUT
                printf("Non compresso = ");
                showbits(currentFormatter->bitCoded[i],2);
            #endif
            fwrite(&currentFormatter->bitCoded[i],sizeof(unsigned char),1,fileptr);
        }else{
            #ifdef DEBUGOUTPUT
                printf("Compresso = ");
                showbits(currentFormatter->bitCoded[i],4);
            #endif
            int j=currentFormatter->bitCoded[i] & 0xFF00;
            j=j>>8;
            #ifdef DEBUGOUTPUT
                printf("1° 8 bit = ");
                showbits(j,2);
            #endif
            fwrite(&j,sizeof(unsigned char),1,fileptr);
            j=currentFormatter->bitCoded[i] & 0xFF;
            #ifdef DEBUGOUTPUT
                printf("2° 8 bit = ");
                showbits(j,2);
            #endif
            fwrite(&j,sizeof(unsigned char),1,fileptr);
        }

    }
    fclose(fileptr);

    return 0;
}