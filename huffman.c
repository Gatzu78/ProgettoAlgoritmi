//
// Created by attilio on 08/10/18.
//

#include <stdlib.h>
#include "huffman.h"
#include "tree.h"

void compressFile(char *inputFile, char *outputFile){

    FILE *fileptr;
    unsigned char *buffer;
    int lungfile;

    fileptr = fopen(inputFile, "rb");     // apertura il file in modalità binaria
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

    compressString(buffer,lungfile, outputFile);

    free(buffer);
}

void decompressFile(char *inputFile, char *outputFile){
    FILE *fp = NULL;
    //fp = readFile(fp,"test1");

    fp = openFile(fp,inputFile,false); //fopen("test1.funny", "rb");

    nodeHuffman headerFile[256] = {0};
    unsigned long cnt;

    fseek(fp, 0, SEEK_END);
    cnt = ftell(fp);

    fseek(fp, 0L, SEEK_SET); //rewind(fp);


    char *buffer;
    buffer = (char *)malloc((cnt+1)*sizeof(char));
    fread(buffer, cnt, 1, fp);

    //salvo tutto il dizionario
    int ind = 0;
    for (int i = 0; i < 256; ++i) {
        if(buffer[i]!=0x00){
            int byteLenght = buffer[i];
            char *elementArray = malloc(sizeof(char) * byteLenght);
            for(int j = 0;j<byteLenght;j++){
                elementArray[j] = "0";
            }
            headerFile [ind] = createNodeHuffman((unsigned char)i,elementArray);
            //headerFile [ind] = buffer[i];
            ind = ind+1;
        }
    }

    sortHuffmanTable(headerFile);

    int count = 1;
    long currentByte = 0x00;
    for (int k = 0; k < 256; k++) {
        if(headerFile[k]==NULL){
            break;
        }
        int dumLenght = len(headerFile[k]->coded);
        if(count == 1){
            count = dumLenght;
        }else if(dumLenght > count){
            currentByte = (currentByte+1)<<dumLenght-count;
            count = dumLenght;
        }else{
            currentByte = currentByte+1;
        }
        for(int i = 0;i<dumLenght;i++){
            if(((currentByte>>(dumLenght-i-1))&0x01)==0x00){
                headerFile[k]->coded[i] = '0';
            }else{
                headerFile[k]->coded[i] = '1';
            }
        }
    }


    nodeTree root = createTree(NULL,0);
    for (int k = 0; k < 256; k++) {
        if (headerFile[k] == NULL) {
            break;
        }
        nodeTree  dumRoot = root;
        for(int t = 0; t < len(headerFile[k]->coded);t++){
            if(headerFile[k]->coded[t]=='0'){
                if(dumRoot->left == NULL){
                    dumRoot->left = createTree(NULL,0);
                }
                dumRoot = dumRoot->left;
            }else{
                if(dumRoot->right == NULL){
                    dumRoot->right = createTree(NULL,0);
                }
                dumRoot = dumRoot->right;
            }
        }
        dumRoot->word = headerFile[k]->key;

    }


    //salvo il gap finale dell'ultimo byte
    char lastByte =  buffer[cnt-1];
    nodeTree  dumRoot = root;

    FILE *fd = NULL;
    fd = openFile(fd,outputFile,true);

    for (long l = 0; l < cnt-257; l++) {
        for (long i = 0; i < 8; i++) {
            if(l == cnt-258 && i+lastByte==8){
                break;
            }
            if(((buffer[256+l]>>(7-i))&0x01)==0x00){
                dumRoot = dumRoot->left;
            }else{
                dumRoot = dumRoot->right;
            }
            if(dumRoot->right == NULL && dumRoot->left == NULL){ //è una foglia
                writeByte(fd,dumRoot->word);
                dumRoot = root;
            }
        }
    }

    fclose(fp);
    fclose(fp);

}