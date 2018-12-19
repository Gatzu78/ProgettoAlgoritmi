
#include "dictionary.h"
#include <stdlib.h>
#include <stdbool.h>
#include <printf.h>
#include "huffman_table.h"
#include "IOFile.h"

#ifndef PROGETTOALGORITMI_TREE_H
#define PROGETTOALGORITMI_TREE_H

struct Tree{
    unsigned char word;
    unsigned long val;
    struct Tree *left;
    struct Tree *right;
};

typedef struct Tree *nodeTree;


nodeTree createTree(unsigned char word, unsigned long val);

nodeTree mergeTree(nodeTree left, nodeTree right);

nodeTree getLeftChild(nodeTree nt);

nodeTree getRightChild(nodeTree nt);

void showtree(nodeTree nt);

char* printArr(int arr[], int n);

void printTable(nodeTree nt, int digit[], int size_digit);


struct FrequencyList{
    struct Tree *tree;
    struct FrequencyList *next;
};

typedef struct FrequencyList *nodeFrequency;

nodeFrequency createFrequencyNode(nodeTree tree);

nodeFrequency addFrequencyNode(nodeFrequency head, nodeTree tree);

nodeFrequency removeFrequency(nodeFrequency head, nodeFrequency nf);

nodeFrequency sortFrequency(nodeFrequency hf);


int populateTree(node dic);

void compressString(char *file, unsigned long lungfile);

void buildTree();

#endif PROGETTOALGORITMI_TREE_H