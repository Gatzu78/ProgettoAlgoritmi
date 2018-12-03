//
// Created by attilio on 22/10/18.
//

#ifndef PROGETTOALGORITMI_WINDOWBUFFER_H
#define PROGETTOALGORITMI_WINDOWBUFFER_H
#include <stdbool.h>



typedef struct bufferNode {
    struct bufferNode* next;
    struct bufferNode* previous;
    struct bufferNode* bufferHead;
    bool isHead;
    bool isTail;
    long offset;
    unsigned char* currentData;
}bufferNode;

unsigned int getGlobalBufferSize();
void setBufferGlobalParameters(int unsigned bufferSize);
bufferNode* createBufferNode(bufferNode* previousNode, int size);
bufferNode* createAheadBuffer(int unsigned bufferSize);
void freeAheadBuffer(bufferNode* headNode);

#endif //PROGETTOALGORITMI_WINDOWBUFFER_H
