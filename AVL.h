//
// Created by attilio on 19/11/18.
//

#ifndef PROGETTOALGORITMI_AVL_H
#define PROGETTOALGORITMI_AVL_H
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#endif //PROGETTOALGORITMI_AVL_H


typedef struct treeNode
{
    int realOffset;
    char* str;
    struct treeNode *left;
    struct treeNode *right;
    int height;
}treeNode;

// A utility function to get maximum of two integers
int max(int a, int b);

