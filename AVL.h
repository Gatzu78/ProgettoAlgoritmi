//
// Created by attilio on 19/11/18.
//

#ifndef PROGETTOALGORITMI_AVL_H
#define PROGETTOALGORITMI_AVL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct treeNode
{
    long realOffset;
    char * str;
    struct treeNode *left;
    struct treeNode *right;
    int height;
}treeNode;

extern treeNode *currentTreeRoot;
extern int currentTreeSize;

// A utility function to get maximum of two integers
int max(int a, int b);

treeNode* newNode(long realOffset,char str[]);
treeNode* insertNode(struct treeNode *node, long realOffset, char *str);
treeNode* deleteNode(treeNode* root, char str[]);
treeNode* searchMatch(treeNode * node, char str[]);



#endif //PROGETTOALGORITMI_AVL_H