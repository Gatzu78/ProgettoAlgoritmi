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
unsigned char *originalFileBegining;
bufferNode * currentBufferHead;
bool useTree = false;
FILE *fileptr;



int comprimiLZSS(unsigned char *buffer, long lungfile, unsigned char *outputBuffer, char * outputPath, unsigned int windowSize, unsigned int lookAheadSize){
    clock_t tStart = clock();
    fileptr = fopen(outputPath, "wb"); //sovrascrive se il file esiste
    originalFileBegining=buffer;
    currentTreeRoot=NULL;
    // il programma è predisposto per lavorare con finestra e look ahead passati da linea di comando, ma in questa implementazione iniziale forzo tutto a valori di default.
    if(windowSize!=FINESTRA){
        windowSize=FINESTRA;
    }
    globalWindowsSize=windowSize;
    if(lookAheadSize!=LOOKAHEAD){
        lookAheadSize=LOOKAHEAD;
    }
    #ifdef DEBUG
        printf("Ho settato le dimensioni della finestra\n");
    #endif
    // creo un puntatore alla posizione attuale, in questo caso l'inizio
    currentPosition = buffer;
    #ifdef DEBUG
        printf("Ho creato un puntatore per tener traccia della posizione nel buffer\n");
    #endif
    //creo la finestra secondo i parametri forniti
    bufferNode *aheadBuffer = createAheadBuffer(lookAheadSize);
    //printf("Ho creato l'ahead buffer\n");
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
    for(currentPositionOffset;currentPositionOffset<lungfile;){
        #ifdef DEBUG
            printf("currentBufferHead è '%c',(%ld)\n",*currentBufferHead->currentData,currentBufferHead->offset);
        #endif
        int evaluatedBytes=0;
        if(useTree){
            evaluatedBytes = evaluateDataTree(currentTreeRoot); //valuta se i dati nel ahead buffer sono già presenti sull'albero albero.
        }else{
            evaluatedBytes=evaluateDataWindow(); //valuta se i dati nel ahead buffer sono già presenti nella finestra.
        }
        shiftOnAheadBuffer(evaluatedBytes); //avanza in base a quanto è riusciuto a codificare.
    }

    long svuotaBuffer=currentBufferHead->offset;
    for(svuotaBuffer;svuotaBuffer<lungfile;svuotaBuffer++){
        int evaluatedBytes=0;
        if(useTree){
            evaluatedBytes=evaluateDataTree(currentTreeRoot); //valuta se i dati nel ahead buffer sono già presenti sull'albero albero.
        }else{
            evaluatedBytes=evaluateDataWindow(); //valuta se i dati nel ahead buffer sono già presenti nella finestra.
        }
        shiftOnAheadBuffer(evaluatedBytes); //avanza in base a quanto è riusciuto a codificare.
    }
    writeFormatter(currentFormatter,fileptr,true);
    free(currentFormatter);
    freeAheadBuffer(aheadBuffer);
    fclose(fileptr);
    printf("Tempo impiegato per compressione: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}

int evaluateDataWindow(){
    unsigned char * tempFileBuffer=originalFileBegining;
    bufferNode * tempBufferNode=currentBufferHead;
    long i = 0;
    if (tempBufferNode->offset>FINESTRA){
        i=tempBufferNode->offset-FINESTRA;
    }
    long lastValidOffsetFound=-1;
    int lastValidBytesFound=0;
    long offsetFound=-1;
    int bytesFound=0;
    for(i;i<currentBufferHead->offset;i++){
         if(*currentBufferHead->currentData=='\000'){
             break;
         }
         if(*tempBufferNode->currentData==tempFileBuffer[i]){
             offsetFound=i;
             bytesFound++;
             if((offsetFound>(currentBufferHead->offset-2))&&(offsetFound<(currentBufferHead->offset+31))){ // caso speciale, vado a vedere anche nell'ahead buffer.
                //Implementare
             }
             if(tempBufferNode->next->isHead==false){
                 tempBufferNode=tempBufferNode->next;
             }else{ //sono arrivato in fondo al buffer circolare ed ho trovato qualcosa
                 if(bytesFound>lastValidBytesFound){ // se il match trovato è meglio di quello precedente
                     lastValidOffsetFound=(offsetFound-bytesFound)+1;
                     lastValidBytesFound=bytesFound;
                 }
                 break;
             }

         } else { // char non uguale
             if(bytesFound>0){ // se loop precedente ha prodotto un match
                 if(bytesFound>lastValidBytesFound){ // se il match trovato è meglio di quello precedente
                     lastValidOffsetFound=(offsetFound-bytesFound)+1;
                     lastValidBytesFound=bytesFound;
                 }
                 offsetFound=-1;
                 bytesFound=0;
                 tempBufferNode=currentBufferHead;
             }
         }
    }
    if (lastValidBytesFound <= 1) { // casi in cui non ho trovato match oppure non conviene comprimere
        currentFormatter=addToFormatter(currentFormatter,0,*currentBufferHead->currentData,fileptr);
        lastValidBytesFound=1;
    } else {
        unsigned int relativeOffset=(unsigned int)(currentBufferHead->offset-lastValidOffsetFound);
        currentFormatter=addToFormatter(currentFormatter,relativeOffset,(unsigned char)lastValidBytesFound,fileptr);
    }
    return lastValidBytesFound;
}

int evaluateDataTree(treeNode *root){
    treeNode *matchTreeNode;
    bufferNode *tempBufferNode=currentBufferHead;
    int evaluatedBytes=0;
    char str[getGlobalBufferSize()+1];
    emptyString(str,(getGlobalBufferSize()+1));
    int i = 0;
    long foundStringOffset=0;
    strncpy(str,(const char *)tempBufferNode->currentData,1);
    for(i;i<getGlobalBufferSize();i++){ //attraverso l'albero alla ricerca di un match con un ciclo for infinito
        matchTreeNode = searchMatch(root,str); //cerco un nodo in cui ci sia un match
        if(matchTreeNode==NULL){
            break; //break dal for di ricerca nell'albero, no match o arrivato in fondo all'albero
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
        currentFormatter=noMatchFound(currentFormatter,currentBufferHead);
        evaluatedBytes=1; // se non ho match oppure ho un solo match, setto di default a 1
    } else{
        currentFormatter=matchFound(currentFormatter,currentBufferHead,evaluatedBytes,foundStringOffset);
    }
    return evaluatedBytes;
}

formatter * noMatchFound(formatter *passedFormatter,bufferNode *aheadBuffer){
    formatter *tempFormatter=addToFormatter(passedFormatter,0,*aheadBuffer->currentData,fileptr); /* Aggiungo il carattere che non ha match al write buffer */
    return tempFormatter;
}

formatter * matchFound(formatter *passedFormatter,bufferNode *aheadBuffer,int evaluatedBytes,long foundStringOffset) {
    // logica per codificare il match di 2 o più caratteri
    long relativeOffset = currentPositionOffset - (foundStringOffset+(int)LOOKAHEAD); //valutare da che riferimento leggere l'offset di decodifica
    formatter *tempFormatter=addToFormatter(currentFormatter,(unsigned int)relativeOffset,(unsigned char)evaluatedBytes,fileptr);
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
        long tempOperator=currentPositionOffset-globalWindowsSize-getGlobalBufferSize();
        unsigned char* tempOffset = originalFileBegining;
        tempOffset=tempOffset+tempOperator;
        char str[getGlobalBufferSize()+1];
        stringBuilder(str,tempOffset);
        currentTreeRoot=deleteNode(currentTreeRoot,str,tempOperator);
        // Rimosso currentTreeSize--
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
        if(useTree){
            deleteFromTree(); // elimina la foglia più vecchia dell'albero, solo quando si riempe.
            addToTree(); // aggiunge una foglia all'albero con il contenuto dell' aheadbuffer
        }
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
            printf("currentBufferHead->offset=%d\ncurrentBufferHead->currentData='%c'\n",(int)currentBufferHead->offset,*currentBufferHead->currentData);
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
            str[i]=offsetFromBuffer[i];
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

int decomprimiLZSS(unsigned char *buffer, long lungfile, char *outputPath){
    clock_t tStart = clock();
    originalFileBegining=buffer;
    currentPosition=buffer;
    fileptr = fopen(outputPath, "wb+"); //sovrascrive se il file esiste
    decoder * currentDecoder=newDecoder();
    for(currentPositionOffset=0;currentPositionOffset<lungfile;){
        if(currentDecoder->count==8){
            writeDecoder(currentDecoder,fileptr);
            deleteDecoder(currentDecoder);
            currentDecoder=newDecoder();
        }
        /* process the buffer to prepare data to be decoded */
        int i=0;
        long tempJump=0;
        unsigned char evaluate= *currentPosition;
        currentPosition++;

        currentPositionOffset++;
        for(i;i<=7;i++){
            char tempEvaluate=evaluate>>((int)7-i);
              currentDecoder->istructionDecoder[i]=(unsigned short)(tempEvaluate & 0x1);
              if(currentDecoder->istructionDecoder[i]==0){
                  currentDecoder->bitCoded[i]=*currentPosition;
                  currentPosition++;
                  currentPositionOffset++;
                  currentDecoder->count++;
                  tempJump+=1;
              }else{
                  currentDecoder->bitCoded[i]=*currentPosition;
                  currentDecoder->bitCoded[i]=currentDecoder->bitCoded[i]<<8;
                  currentPosition++;
                  currentPositionOffset++;
                  currentDecoder->bitCoded[i]+=*currentPosition;
                  currentPosition++;
                  currentPositionOffset++;
                  currentDecoder->count++;
                  tempJump+=2;
              }
        }

    }
    fclose(fileptr); //chiude il file
    printf("Tempo impiegato per decompressione: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}