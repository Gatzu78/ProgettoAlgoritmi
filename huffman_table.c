//
// Created by Simone on 05.11.18.
//

#include <stdlib.h>
#include <printf.h>
#include <memory.h>
#include <stdbool.h>
#include "huffman_table.h"

/**
 * viene ritornato il nodo di huffman ricevendo come parametro l'array in cui cercare la key
 */
nodeHuffman getHuffmanNodeByKey(nodeHuffman arr[], int arr_size, unsigned char key){
    for(int i=0; i<arr_size;i++){
        if(arr[i] == NULL){
            break;
        }
        if(arr[i]->key==key){
            return arr[i];
        }
    }
}

/**
 * viene utilizzata per il debug, per stampare a terminale la tabella
 */
void printHuffmanTable(nodeHuffman arr[], int arr_size){
    for(int i=0; i<arr_size;i++){
        if(arr[i] == NULL){
            break;
        }
        printf("%c \t %s\n",arr[i]->key,arr[i]->coded);
    }
}

/**
 * funzione utilizzata per creare un nuovo nodo per l'header
 */
nodeHuffman createNodeHuffman(unsigned char key, char *coded){
    nodeHuffman temp;
    temp = (nodeHuffman)malloc(sizeof(struct HuffmanTable));
    temp->key = key;
    temp->coded = coded;
    return temp;
}

/**
 * funzione che passata una stringa, ritorna la sua lunghezza
 */
unsigned len(const char* s)
{
    int count = 0;
    int i = 0;
    while (*(s + i) != 0)
    {
        count++;
        i++;
    }
    return count;
}

/**
 * Funzione che, tramite l'ausilio di un insertion sort, riordina per lunghezza e poi in ordine alfabetico, tutta l'heaer
 * @param arr
 * @return
 */
nodeHuffman* sortHuffmanTable(nodeHuffman arr[]){
    for(int i=0; i<256;i++){
        if(arr[i] == NULL){
            break;
        }
        nodeHuffman value = arr[i];
        int j = i-1;
        while(j>=0  && (len(value->coded)<len(arr[j]->coded) || (len(value->coded)== len(arr[j]->coded) && arr[j]->key>value->key))){
            arr[j+1] = arr[j];
            j=j-1;
        }
        arr[j+1]=value;
    }
    return arr;
}

/**
 * funzione utilizzata per creare l'header
 */
void createHeader(nodeHuffman arr[], char headerToSave[]){
    int cnt = 1; //contatore dei bit
    long currentByte = 0;

    for(int i=0; i<256;i++){
        if(arr[i] == NULL){
            break;
        }
        char *test = arr[i]->coded;
        int dumLenght = len(test);
        dumLenght = dumLenght==0?1:dumLenght;
        if(cnt == 1){
            cnt = dumLenght;
        }else if(dumLenght > cnt){
            currentByte = (currentByte+1)<<dumLenght-cnt;
            cnt = dumLenght;
        }else{
            currentByte = currentByte+1;
        }
        char *code = malloc(sizeof(char) * cnt);

        for(int l=0;l<cnt;l++){
            if(((currentByte>>(cnt-l-1))&0x01)==0x00){
                code[l] = '0';
            }else{
                code[l] = '1';
            }
        }

        int index = (int)arr[i]->key;
        arr[i]->coded = code;
        headerToSave[index] = cnt;
    }
}
