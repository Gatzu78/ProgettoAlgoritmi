//
// Created by attilio on 01/10/18.
//

#ifndef PROGETTOALGORITMI_LZSS_H
#define PROGETTOALGORITMI_LZSS_H
#include "aheadbuffer.h"
#include "utils.h"
#define FINESTRA    16    //2^16 4096 Offset
#define LOOKAHEAD    5    //2^5 Lenght


int comprimi(unsigned char *buffer, long lungfile, char *outPutFileName,int windowSize, int lookAheadSize);
int decomprimi(unsigned char *buffer, long lungfile, char *outPutFileName);
void shiftOnSlidingWindow(unsigned char *currentPosition, bufferNode *slidingWindow);
void loadOnAheadBuffer(unsigned char *currentPosition, bufferNode *slidingWindow);

#endif //PROGETTOALGORITMI_LZSS_H
