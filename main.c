#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lzss.h"
#include "huffman.h"

int main(int argc, char *argv[]) {
    if(argc!=4){
        printf("manca un argomento per la corretta esecutione dell'applicativo\n");
        return 0;
    }
    /*DEBUG*/
    printf("%s\n",argv[1]);
    /*DEBUG*/
    if ((strcmp(argv[1],"-C")!=0) && (strcmp(argv[1],"-D")!=0)) {
        printf("non è stato specificato un argomento valido\n");
        return 0;
    }

    FILE *fileptr;
    char *buffer;
    long lungfile;
    char *outputBuffer;
    /*
        AGGIUNGERE un controllo se il file esiste.
    */
    fileptr = fopen(argv[2], "rb");     // apertura il file in modalità binaria
    fseek(fileptr, 0, SEEK_END);        // vado alla fine del file
    lungfile = ftell(fileptr)+1;        // memorizza l'offset del file (quanti caratteri ci sono) + 1
    rewind(fileptr);                    // torna al'inizio del file

    buffer = ( unsigned char *)malloc((lungfile)*sizeof(unsigned char));    // alloca abbastanza memoria per contenere il file previsto  anche \0
    fread(buffer, lungfile, 1, fileptr);                                    // legge il file e lo carica in memoria (copia sul buffer)
    fclose(fileptr);                                                        // Chiude il file

    int i = 0;
    for(i = 2; i<lungfile; i++){
        printf("%c -> %d\n", buffer[i], buffer[i]);
    }

    if(strcmp(argv[1],"-C")==0){ //Routine di compressione
        comprimi(buffer,lungfile,outputBuffer,0,0);
    }

    if(strcmp(argv[1],"-D")==0){ //Routine di decompressione

    }

    return 0;
}

