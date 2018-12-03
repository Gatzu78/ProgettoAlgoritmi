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
    bool isBuffer;
    bool isHead;
    bool isTail;
    int offset;
    char* currentData;
}bufferNode;

unsigned int getGlobalWindowSize();
unsigned int getGlobalBufferSize();
void setBufferGlobalParameters(int unsigned windowSize,int unsigned bufferSize);
bufferNode* createBufferNode(bufferNode* previousNode, int offset);
bufferNode* createSlidingWindow(int unsigned windowSize,int unsigned bufferSize);
void freeAheadBuffer(bufferNode *headNode);

#endif //PROGETTOALGORITMI_WINDOWBUFFER_H
