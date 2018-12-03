//
// Created by attilio on 22/10/18.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aheadbuffer.h"

static unsigned int globalBufferSize = 0;

//Leggo i parametri globali
unsigned int getGlobalBufferSize(){
    return globalBufferSize;
}

// Setta parametri globali per il corretto funzionamento della finestra.
void setBufferGlobalParameters(int unsigned bufferSize){
    globalBufferSize=bufferSize;
    printf("globalAheadBufferSize is equal to %d\n",globalBufferSize);
}

// Crea un nodo della linked list e lo aggancia al nodo precedente.
bufferNode* createBufferNode(bufferNode *previousNode, int size){
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
    newNode->next=NULL;
    newNode->offset=0;
    newNode->isTail=true;

    if(size==1){
        newNode->next=previousNode->bufferHead;
        newNode->bufferHead->previous=newNode;
    }

    return newNode;
}

// crea la linked list usata come finestra e look ahead della dimensione opportuna a seconda dei parametri passati
bufferNode* createAheadBuffer(int unsigned bufferSize){
    setBufferGlobalParameters(bufferSize);
    printf("Ho settato la variabile globale per la dimensione del ahead buffer  =%d\n",getGlobalBufferSize());
    int size = globalBufferSize;
    bufferNode *tempNode=(bufferNode*)calloc(size,sizeof(bufferNode));
    printf("Ho settato un puntatore NULL da passare alla creazione dell'ahead buffer\n");
    for(size;size>0;size--){
        printf("Numero di iterazioni restanti per la creazione della finestra -> %d\n",size);
        tempNode=createBufferNode(tempNode,(size));
    }
    return tempNode->bufferHead; //restituisce la testa della linked list
}

void freeAheadBuffer(bufferNode *headNode){

    printf("Libero la memoria della finestra\n");
    free(headNode);
}