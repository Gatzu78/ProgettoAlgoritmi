//
// Created by attilio on 01/10/18.
//
#include <stdio.h>
#include "lzss.h"

long currentPositionOffset = 0; // tiene traccia della posizione del char letto all'interno del buffer
int currentTreeSize=0;
unsigned char *currentPosition;
treeNode *currentTreeRoot;
formatter *currentFormatter;
unsigned int globalWindowsSize;



int comprimiLZSS(unsigned char *buffer, long lungfile, char *outPutFileName, unsigned int windowSize, unsigned int lookAheadSize){
    clock_t tStart = clock();
    currentTreeRoot=NULL;
    printf("Questo è buffer = %c\n",buffer[1]);
    // il programma è predisposto per lavorare con finestra e look ahead passati da linea di comando, ma in questa implementazione iniziale forzo tutto a valori di default.
    if(windowSize!=FINESTRA){
        windowSize=FINESTRA;
    }
    globalWindowsSize=windowSize;
    if(lookAheadSize!=LOOKAHEAD){
        lookAheadSize=LOOKAHEAD;
    }
    printf("Ho settato le dimensioni della finestra\n");
    // creo un puntatore alla posizione attuale, in questo caso l'inizio
    currentPosition = buffer;
    printf("Ho creato un puntatore per tener traccia della posizione nel buffer\n");
    //creo la finestra secondo i parametri forniti
    bufferNode *aheadBuffer = createAheadBuffer(lookAheadSize);
    printf("Ho creato l'ahead buffer\n");
    //chiamo il caricamento del buffer
    loadOnAheadBuffer(aheadBuffer);
    /* creo un formater usato per prepare i dati in output*/
    currentFormatter = newFormatter();
    currentFormatter=noMatchFound(currentFormatter,aheadBuffer);
    shiftOnAheadBuffer(1);
    #ifdef DEBUG
        printf("Il file caricato in memoria è composto da %ld char(byte)\n",lungfile);
        printf("currentPositionSize è di %ld char(byte)\n",currentPositionOffset);
    #endif
    for(currentPositionOffset;currentPositionOffset<lungfile-1;){
        //Verificare il reale numero di cicli necessari, soprattutto quando mi avvicino alla fine del file.
        #ifdef DEBUG
            printf("Sono alla posizione %ld di %ld, il  valore è: %c\n",currentPositionOffset,lungfile-1,*currentPosition);
        #endif
        int evaluatedBytes = evaluateData(currentTreeRoot); //valuta se i dati nel ahead buffer sono già presenti sull'albero albero.
        shiftOnAheadBuffer(evaluatedBytes); //avanza in base a quanto è riusciuto a codificare.
    }

    freeAheadBuffer(aheadBuffer);
    printf("Tempo impiegato per compressione: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
};

int evaluateData(treeNode *root){
    treeNode *matchTreeNode;
    bufferNode *tempBufferNode=currentBufferHead;
    int evaluatedBytes=0;
    char str[getGlobalBufferSize()+1];
    emptyString(str,(getGlobalBufferSize()+1));
    int i = 0;
    long foundStringOffset=0;
    strncpy(str,(const char *)tempBufferNode->currentData,1);
    for(i;i<getGlobalBufferSize();i++){
        matchTreeNode = searchMatch(root,str);
        if(matchTreeNode==NULL){
            break;
        } else{ /* Trovato un match nell'albero */
            evaluatedBytes++;
            if(evaluatedBytes==1){
                foundStringOffset=matchTreeNode->realOffset;
            }
            tempBufferNode=tempBufferNode->next;
            strncat(str,(const char*)tempBufferNode->currentData,1);
        }
    }
    if (evaluatedBytes<=1){
        currentFormatter=noMatchFound(currentFormatter,tempBufferNode);
        evaluatedBytes=1; // se non ho match oppure ho un solo match, setto di default a 1
    } else{
        currentFormatter=matchFound(currentFormatter,currentBufferHead,evaluatedBytes,foundStringOffset);
    }
    return evaluatedBytes;
}

formatter * noMatchFound(formatter *passedFormatter,bufferNode *aheadBuffer){
    formatter *tempFormatter=addToFormatter(passedFormatter,0,*aheadBuffer->currentData); /* Aggiungo il carattere che non ha match al write buffer */

    return tempFormatter;
}

formatter * matchFound(formatter *passedFormatter,bufferNode *aheadBuffer,int evaluatedBytes,long foundStringOffset) {
    // logica per codificare il match di 2 o più caratteri
    long relativeOffset = currentPositionOffset - foundStringOffset; //valutare da che riferimento leggere l'offset di decodifica
    formatter *tempFormatter=addToFormatter(currentFormatter,(unsigned int)relativeOffset,(unsigned char)evaluatedBytes);
    return tempFormatter;
}

/* cancella dall'albero la foglia più vecchia finestra+1 */
void addToTree(){
    char str[getGlobalBufferSize()+1];
    stringBuilder(str,NULL);
    currentTreeRoot = insertNode(currentTreeRoot, (currentPositionOffset-getGlobalBufferSize()), str);
    currentTreeSize++;
}

/* cancella dall'albero la foglia più vecchia finestra+1 */
void deleteFromTree(){
    if (currentTreeSize>globalWindowsSize){
        unsigned char* tempOffset = currentPosition -(globalWindowsSize+getGlobalBufferSize()+1);
        char str[getGlobalBufferSize()+1];
        stringBuilder(str,tempOffset);
        currentTreeRoot=deleteNode(currentTreeRoot,str);
        currentTreeSize--;
    }
}

/*
 * Riempo il look ahead buffer con i dati provenienti dal buffer creato con il file.
 * Viene eseguito solo una volta all'inizio.
 */
void loadOnAheadBuffer(bufferNode *aheadBuffer){
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
        aheadBuffer=aheadBuffer->next;
    }
}
/*
 * Fa scorrere i dati da comprimere sulla finestra.
 * Aggiorna il puntatore e il contatore sui dati da comprimere.
 * Lo shift dipende da quanti caratteri sono riuscito a matchare
 */
void shiftOnAheadBuffer(int detectedChars) {
    bufferNode* tempNode = currentBufferHead;
    for(detectedChars;detectedChars>0;detectedChars--){
        addToTree(); // aggiunge una foglia all'albero con il contenuto dell' aheadbuffer
        deleteFromTree(); // elimina la foglia più vecchia dell'albero, solo quando si riempe.
        tempNode->isHead=false;
        tempNode->next->isHead=true;
        tempNode->isTail=true;
        tempNode->previous->isTail=false;
        tempNode->currentData=currentPosition;
        tempNode->offset=currentPositionOffset;
        tempNode=tempNode->next;
        currentBufferHead=tempNode;
        currentPosition++;
        currentPositionOffset++;
        #ifdef DEBUG
            printf("currentBufferHead->offset=%d\n",(int)currentBufferHead->offset);
        #endif
    }
}
/* Crea un stringa dal circular buffer o dall'offset passato*/
void stringBuilder(char *str, unsigned char *offsetFromBuffer){
    emptyString(str,(getGlobalBufferSize()+1));
    unsigned int i = 0;
    if (offsetFromBuffer==NULL){
        bufferNode *tempNode = currentBufferHead;
        for (i;i<getGlobalBufferSize();i++){
            str[i]=*tempNode->currentData;
            tempNode=tempNode->next;
        }
    } else { /* creare una stringa per fare la delete sull'albero */
        for (i;i<getGlobalBufferSize();i++){
            str[i]=*offsetFromBuffer;
            offsetFromBuffer=offsetFromBuffer++;
        }
    }
    #ifdef DEBUG
        i=0;
        printf("str vale=");
        for(i;i<getGlobalBufferSize();i++){
            printf(" '%c' ",str[i]);
        }
        printf("\n");
    #endif
}

int decomprimiLZSS(unsigned char *buffer, long lungfile, char *outPutFileName){
    return 0;
}