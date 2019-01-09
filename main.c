#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "lzss.h"
#include "huffman.h"


int main(int argc, char *argv[]) {
    if(argc!=4){
        printf("manca un argomento per la corretta esecutione dell'applicativo\n");
        return 0;
    }
    #ifdef DEBUG
    printf("È stato passato l'argomento %s\n",argv[1]);
    #endif
    if ((strcmp(argv[1],"-C")!=0) && (strcmp(argv[1],"-D")!=0)) {
        printf("non è stato specificato un argomento valido\n");
        return 0;
    }

    FILE *fileptr;
    unsigned char *buffer;
    long lungfile;
    unsigned char *outputBuffer;
    char * outputPath=argv[3];
    #ifdef DEBUG
        printf("Il percorso di output è %s\n", outputPath);
    #endif
    /*
        Controllo se il file è vuoto.
    */
    if (!(fileptr = fopen(argv[2], "rb"))){ // apertura il file in modalità binaria
        printf("Non è stato possibile aprire il file per leggere i dati, file vuoto o inesistente;");
    }else{
        fseek(fileptr, 0, SEEK_END);        // vado alla fine del file
        lungfile = ftell(fileptr)+1;        // memorizza l'offset del file (quanti caratteri ci sono) + 1
        rewind(fileptr);                    // torna al'inizio del file

        buffer = ( unsigned char *)malloc((lungfile)*sizeof(unsigned char));    // alloca abbastanza memoria per contenere il file previsto  anche \0
        fread(buffer, lungfile, 1, fileptr);                                    // legge il file e lo carica in memoria (copia sul buffer)
        fclose(fileptr);                                                        // Chiude il file
        #ifdef DEBUG
            printf("Il file è lungo %ld\n",lungfile);
            unsigned char * evaluatingPtr = buffer+lungfile-1;
            printf("Il char in fondo al buffer è '%u'\n",*evaluatingPtr);
            int i = 0;
            for(i = 0; i<lungfile; i++){
            printf("%c -> %d\n", buffer[i], buffer[i]);
            }
        #endif
        if(strcmp(argv[1],"-C")==0){ //Routine di compressione
            comprimiLZSS(buffer, lungfile, outputBuffer, outputPath, 0, 0);
        }

        if(strcmp(argv[1],"-D")==0){ //Routine di decompressione
            decomprimiLZSS(buffer,lungfile,outputPath);
        }
    }


    return 0;
}

