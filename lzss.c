//
// Created by attilio on 01/10/18.
//
#include <stdio.h>
#include "lzss.h"
#define DEBUG

long currentPositionOffset = 0; // tiene traccia della posizione del char letto all'interno del buffer



int comprimi( unsigned char *buffer, long lungfile, char *outPutFileName,int windowSize, int lookAheadSize){
    printf("Questo è buffer = %c\n",buffer[1]);
    // Se i valori impostati di finestra a read ahead buffer sono sotto al minimo li porto al valore default.
    if(windowSize<FINESTRA){
        windowSize=FINESTRA;
    }
    if(lookAheadSize<LOOKAHEAD){
        lookAheadSize=LOOKAHEAD;
    }
    printf("Ho settato le dimensioni della finestra\n");
    // creo un puntatore alla posizione attuale, in questo caso l'inizio
    unsigned char* currentPosition = buffer;
    printf("Ho creato un puntatore per tener traccia della posizione nel buffer\n");
    //creo la finestra secondo i parametri forniti
    bufferNode *aheadBuffer = createAheadBuffer(lookAheadSize);
    printf("Ho creato l'ahead buffer\n");
    //chiamo il caricamento del buffer
    loadOnAheadBuffer(currentPosition, aheadBuffer);
    printf("Il file caricato in memoria è composto da %ld char(byte)\n",lungfile);
    printf("currentPositionSize è di %ld char(byte)\n",currentPositionOffset);
    /*for(currentPositionSize;currentPositionSize<=lungfile;currentPosition++){
        //Verificare il reale numero di cicli necessari, soprattutto quando mi avvicino alla fine del file.

        printf("Sono alla poszione %ld di %ld\n",currentPositionSize,lungfile);
        //shiftOnSlidingWindow(currentPosition,aheadBuffer);
        currentPositionSize++;
    }*/
    freeAheadBuffer(aheadBuffer);
    return 0;
};
/*
 * Riempo il look ahead buffer con i dati provenienti dal buffer creato con il file.
 * Viene eseguito solo una volta all'inizio.
 */
void loadOnAheadBuffer(unsigned char *currentPosition, bufferNode *aheadBuffer){
    int i = getGlobalBufferSize();
    for (i;i>0;i--){
        #ifdef DEBUG
            printf("%s - Carattere trovato nel file = %c (%d)\n",timeStamp(),*currentPosition,*currentPosition);
        #endif
        aheadBuffer->currentData=currentPosition;
        aheadBuffer->offset=currentPositionOffset;
        #ifdef DEBUG
            printf("%s - Carattere caricato nel buffer = %c (%d)\n",timeStamp(),*aheadBuffer->currentData,*aheadBuffer->currentData);
        #endif
        currentPosition++;
        currentPositionOffset++;
    }
}
/*
 * Fa scorrere il i dati da comprimere sulla finestra.
 * Aggiorna il puntatore e il contatore sui dati da comprimere.
 */
void shiftOnSlidingWindow(unsigned char *currentPosition, bufferNode *slidingWindow){
    bufferNode *tempNode = slidingWindow;
    do{

    }while(tempNode->next==NULL);
}


int decomprimi(unsigned char *buffer, long lungfile, char *outPutFileName){
    return 0;
}