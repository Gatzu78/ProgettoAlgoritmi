//
// Created by attilio on 01/10/18.
//

#ifndef PROGETTOALGORITMI_LZSS_H
#define PROGETTOALGORITMI_LZSS_H
#include "aheadbuffer.h"
#include "AVL.h"
#include "utils.h"
#include "datawriter.h"
#include "datadecoder.h"
#define FINESTRA    2048    //2^11 = 2048 Offset
#define LOOKAHEAD    32    //2^5 = 32 Lenght


int comprimiLZSS(unsigned char *buffer, long lungfile, unsigned char *outputBuffer, char * outputPath, unsigned int windowSize, unsigned int lookAheadSize);
int decomprimiLZSS(unsigned char *buffer, long lungfile, char *outputPath);
void shiftOnAheadBuffer(int detectedChars);
void loadOnAheadBuffer(bufferNode *aheadBuffer);
void stringBuilder(char *str, unsigned char *offsetFromBuffer);
int evaluateData(treeNode *root);
formatter * noMatchFound(formatter *passedFormatter,bufferNode *aheadBuffer);
formatter * matchFound(formatter *passedFormatter,bufferNode *aheadBuffer,int evaluatedBytes,long foundStringOffset);
#endif //PROGETTOALGORITMI_LZSS_H
