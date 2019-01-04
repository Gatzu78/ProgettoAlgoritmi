//
// Created by attilio on 17/12/18.
//

#ifndef PROGETTOALGORITMI_DATAWRITER_H
#define PROGETTOALGORITMI_DATAWRITER_H



extern int formatterWriteSelector;

/* formatter usato per preparare il dato da scrivere */
typedef struct formatter{
    unsigned int count;
    unsigned char bitIndex[8];
    unsigned int  bitCoded[8]; // per l'implementazione 2048 - 32, 16 bit, basterebbe un unsigned short
}formatter;

formatter * newFormatter();
int deleteFormatter(formatter * currentFormatter);
formatter * addToFormatter(formatter * currentFormatter, unsigned int offset, unsigned char posRight);
int writeFormatter(formatter * currentFormatter);
int writeToBuffer(formatter * currentFormatter);
int writeToFile(formatter * currentFormatter);


#endif //PROGETTOALGORITMI_DATAWRITER_H
