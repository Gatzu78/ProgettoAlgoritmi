//
// Created by Simone on 05.11.18.
//

#include <stdlib.h>
#include <printf.h>
#include <memory.h>
#include <stdbool.h>
#include "huffman_table.h"

nodeHuffman getHuffmanNodeByKey(nodeHuffman arr[], int arr_size, char key){
    for(int i=0; i<arr_size;i++){
        if(arr[i] == NULL){
            break;
        }
        if(arr[i]->key==key){
            return arr[i];
        }
    }
}

void printHuffmanTable(nodeHuffman arr[], int arr_size){
    for(int i=0; i<arr_size;i++){
        if(arr[i] == NULL){
            break;
        }
        printf("%c \t %s\n",arr[i]->key,arr[i]->coded);
    }
}

nodeHuffman createNodeHuffman(char key, char *coded){
    nodeHuffman temp;
    temp = (nodeHuffman)malloc(sizeof(struct HuffmanTable));
    temp->key = key;
    temp->coded = coded;
    return temp;
}

unsigned len(const char* s)
{
    int count = 0;; int i = 0;
    while (*(s + i) != 0)
    {
        count++;
        i++;
    }
    return count;
}

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

void createHeader(nodeHuffman arr[], char headerToSave[]){
    int cnt = 1; //contatore dei bit
    char currentByte = 0x00;

    for(int i=0; i<256;i++){
        if(arr[i] == NULL){
            break;
        }
        int dumLenght = len(arr[i]->coded);
        if(cnt == 1){
            cnt = dumLenght;
        }else if(dumLenght > cnt){
            cnt = dumLenght;
            currentByte = (currentByte+1)<<1;
        }else{
            cnt = dumLenght;
            currentByte = currentByte+1;
        }

        char word[8] = {0};
        char *code = malloc(sizeof(char) * cnt);

        for(int l=0;l<cnt;l++){
            if(((currentByte>>(cnt-l-1))&0x01)==0x00){
                strcat(word, "0");
                code[l] = '0';
            }else{
                strcat(word, "1");
                code[l] = '1';
            }
        }

        int index = (int)arr[i]->key;
        arr[i]->coded = code;
        headerToSave[index] = cnt;
    }
    //headerToSave =  header;
}
