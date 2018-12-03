//
// Created by attilio on 22/10/18.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "windowbuffer.h"

static unsigned int globalWindowSize = 0;
static unsigned int globalBufferSize = 0;

//Leggo i parametri globali
unsigned int getGlobalWindowSize(){
    return globalWindowSize;
}
unsigned int getGlobalBufferSize(){
    return globalBufferSize;
}
// Setta parametri globali per il corretto funzionamento della finestra.
void setBufferGlobalParameters(int unsigned windowSize,int unsigned bufferSize){
    globalWindowSize=windowSize;
    printf("globalWindowSize is equal to %d\n",globalWindowSize);
    globalBufferSize=bufferSize;
    printf("globalBufferSize is equal to %d\n",globalBufferSize);
}
// Crea un nodo della linked list e lo aggancia al nodo precedente.
bufferNode* createBufferNode(bufferNode *previousNode,int offset){
    bufferNode* newNode;
    if((previousNode->previous==NULL)&&previousNode->isHead==false){
        newNode = previousNode;
        printf("Questo è il primo nodo (HEAD)\n");
        newNode->isHead=true;
        newNode->bufferHead=newNode;

    } else {
        newNode = previousNode+ sizeof(bufferNode);
        printf("Questo non è il primo nodo\n");
        newNode->isHead=false;
        newNode->bufferHead=previousNode->bufferHead;
        newNode->previous=previousNode;
        previousNode->next=newNode;
        previousNode->isTail=false;
    }
    newNode->isBuffer=false;
    newNode->next=NULL;
    newNode->offset=offset;
    newNode->isTail=true;
    if(offset<=0) {
        newNode->isBuffer = true;
    }
    return newNode;
}
// crea la linked list usata come finestra e look ahead della dimensione opportuna a seconda dei parametri passati
bufferNode* createSlidingWindow(int unsigned windowSize,int unsigned bufferSize){
    setBufferGlobalParameters(windowSize,bufferSize);
    printf("Ho settato le variabili globali della dimensione finestra  =%d+%d\n",getGlobalWindowSize(),getGlobalBufferSize());
    int size = globalWindowSize+globalBufferSize;
    bufferNode *tempNode=(bufferNode*)calloc(size,sizeof(bufferNode));
    printf("Ho settato un puntatore NULL da passare alla creazione finestra\n");
    for(size;size>0;size--){
        printf("Numero di iterazioni restanti per la creazione della finestra -> %d\n",size);
        tempNode=createBufferNode(tempNode,(size-globalBufferSize));
    }
    return tempNode->bufferHead; //restituisce la testa della linked list
}

void freeAheadBuffer(bufferNode *headNode){
    printf("Libero la memoria della finestra\n");
    printf("L'offset della primo nodo è %d, isHead=%d\n",headNode->offset,headNode->isHead);
    free(headNode);
}