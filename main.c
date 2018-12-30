#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lzss.h"
#include "huffman.h"
#include "tree.h"
#include <time.h>


int main(int argc, char *argv[]) {
    /*
    node head = NULL;

    char testo[] = "faaaaaaabbbbbcccddededed";
    int text_size = sizeof(testo)/sizeof(testo[0]);
    for(long i = 0; i<text_size-1;i++){
        head = incrementValue(head,testo[i]);
    }

    populateTree(head);

    compressString(testo);

    buildTree();

    return 0; */
    /*if(argc!=4){
        printf("manca un argomento per la corretta esecutione dell'applicativo\n");
        return 0;
    }
    if ((strcmp(argv[1],"-C")!=0)||(strcmp(argv[1],"-D")!=0)) {
        printf("non è stato specificato un argomento valido\n");
        return 0;
    }*/
    compressFile("32k_ff","test1.funny");
    decompressFile("test1.funny","32k_ff2");
/*
    FILE *fileptr;
    unsigned char *buffer;
    int lungfile;

    fileptr = fopen("immagine.tiff", "rb");     // apertura il file in modalità binaria
    fseek(fileptr, 0, SEEK_END);        // vado alla fine del file
    lungfile = ftell(fileptr)+1;        // memorizza l'offset del file (quanti caratteri ci sono) + 1
    rewind(fileptr);                    // torna al'inizio del file

    buffer = (unsigned char *)malloc((lungfile)*sizeof(unsigned char));    // alloca abbastanza memoria per contenere il file previsto  anche \0
    fread(buffer, lungfile, 1, fileptr);                                    // legge il file e lo carica in memoria (copia sul buffer)
    fclose(fileptr);                                                        // Chiude il file

    node head = NULL;


    for(long i = 0; i<lungfile-1;i++){
        head = incrementValue(head,buffer[i]);
    }

    populateTree(head);

    compressString(buffer,lungfile, "test1.funny");

    free(buffer);

    time_t start_t, end_t;
    double diff_t;

    time(&start_t);
*/
  //  buildTree("test1.funny");
/*
    time(&end_t);
    diff_t = difftime(end_t, start_t);

    printf("Execution time = %f\n", diff_t);
*/
    /*
    if(strcmp(argv[1],"-C")==0){ //Routine di compressione

    }

    if(strcmp(argv[1],"-D")==0){ //Routine di decompressione

    }*/

    return 0;
}

