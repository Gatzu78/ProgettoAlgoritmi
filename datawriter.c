//
// Created by attilio on 17/12/18.
//
#include <stdlib.h>
#include "datawriter.h"
#include "utils.h"

int formatterWriteSelector = 0;

formatter * newFormatter(){
    formatter * currentFormatter=(formatter*)malloc(sizeof(formatter));
    currentFormatter->count=0;
    return currentFormatter;
}

int deleteFormatter(formatter * currentFormatter){
    free(currentFormatter);
};

formatter * addToFormatter(formatter * currentFormatter, unsigned int offset, unsigned char data){
    if(currentFormatter->count>7){
        writeFormatter(currentFormatter);
        deleteFormatter(currentFormatter);
        currentFormatter=newFormatter();
    }
    #ifdef DEBUG
        printf("currentFormatter->count vale %d\n",currentFormatter->count);
        if (offset==0){
            printf("dato non compressa di valore -> %c\n",data);
        }else{
            printf("dato compresso di valore offset %d e spostamento %d\n",offset,(unsigned int)data);
        }
    #endif
    if(offset==0){
        currentFormatter->bitIndex[currentFormatter->count]=0;
        currentFormatter->bitCoded[currentFormatter->count]=data;
        currentFormatter->count++;
        #ifdef DEBUG
            showbits(currentFormatter->bitCoded[currentFormatter->count]); // stampa la codifica
        #endif
    }else{
        currentFormatter->bitIndex[currentFormatter->count]=1;
        currentFormatter->bitCoded[currentFormatter->count]=((offset<<5)+((unsigned int)data));
        currentFormatter->count++;
        #ifdef DEBUG
            showbits(currentFormatter->bitCoded[currentFormatter->count]); // stampa la codifica
        #endif
    }
    return currentFormatter;
};

int writeFormatter(formatter * currentFormatter){
    if(formatterWriteSelector==0){
        writeToBuffer(currentFormatter);
    } else {
        writeToFile(currentFormatter);
    }
};
int writeToBuffer(formatter * currentFormatter){
    return 0;

}
int writeToFile(formatter * currentFormatter){
    return 0;
}