//
// Created by attilio on 15/10/18.
//
#include <string.h>
#include <stdlib.h>
#include "stringtree.h"

Nodo* newNodo(char* data){
    Nodo* newNodo = (Nodo*)malloc(sizeof(Nodo));
    newNodo ->matchString=data;
    newNodo->isMatch=0;
    return newNodo;
    }
void insertNodo(char* data){

}