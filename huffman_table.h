//
// Created by Simone on 05.11.18.
//

#ifndef PROGETTOALGORITMI_HUFFMANTABLE_H
#define PROGETTOALGORITMI_HUFFMANTABLE_H


struct HuffmanTable{
    unsigned char key;
    char *coded;
};

typedef struct HuffmanTable *nodeHuffman;


nodeHuffman getHuffmanNodeByKey(nodeHuffman arr[], int arr_size, unsigned char key);

nodeHuffman createNodeHuffman(unsigned char key, char *coded);

void printHuffmanTable(nodeHuffman arr[], int arr_size);

unsigned int len(const char* s);

nodeHuffman* sortHuffmanTable(nodeHuffman arr[]);
void createHeader(nodeHuffman arr[], char *headerToSave);

#endif PROGETTOALGORITMI_HUFFMANTABLE_H