//
// Created by Simone on 05.11.18.
//

#ifndef PROGETTOALGORITMI_HUFFMANTABLE_H
#define PROGETTOALGORITMI_HUFFMANTABLE_H


struct HuffmanTable{
    char key;
    char *coded;
};

typedef struct HuffmanTable *nodeHuffman;


nodeHuffman getHuffmanNodeByKey(nodeHuffman arr[], int arr_size, char key);

nodeHuffman createNodeHuffman(char key, char *coded);

void printHuffmanTable(nodeHuffman arr[], int arr_size);

unsigned len(const char* s);

nodeHuffman* sortHuffmanTable(nodeHuffman arr[]);
void createHeader(nodeHuffman arr[], char *headerToSave);

#endif PROGETTOALGORITMI_HUFFMANTABLE_H