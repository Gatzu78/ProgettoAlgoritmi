//
// Created by attilio on 15/10/18.
//
#ifndef PROGETTOALGORITMI_STRINGTREE_H
#define PROGETTOALGORITMI_STRINGTREE_H

struct Nodo {
    struct Nodo *right;
    struct Nodo *left;
    int isMatch;
    char* matchString;
};
typedef struct Nodo Nodo;

Nodo* newNodo(char data[]);



#endif //PROGETTOALGORITMI_STRINGTREE_H


