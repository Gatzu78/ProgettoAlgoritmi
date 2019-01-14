//
// Created by attilio on 19/11/18.
//

#ifndef PROGETTOALGORITMI_AVL_H
#define PROGETTOALGORITMI_AVL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRSIZE    32

typedef struct treeNode
{
    long realOffset;
    char *str;
    struct treeNode *left;
    struct treeNode *right;
    int height;
}treeNode;

extern treeNode *currentTreeRoot;
extern int currentTreeSize;

int max(int a, int b); // A utility function to get maximum of two integers
treeNode* newNode(long realOffset,char str[]);
treeNode* insertNode(struct treeNode *node, long realOffset, char *str);
treeNode* deleteNode(treeNode* root, char str[], long checkOffset);
treeNode* searchMatch(treeNode * node, char str[]);
treeNode * minValueNode(treeNode* node);
int getBalance(struct treeNode *N);
treeNode *rightRotate(struct treeNode *y);
treeNode *leftRotate(struct treeNode *x);



#endif //PROGETTOALGORITMI_AVL_H