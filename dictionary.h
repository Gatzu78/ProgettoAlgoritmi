#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct Dictionary{
    unsigned char key;
    unsigned long long value;
    struct Dictionary *next;
};

typedef struct Dictionary *node;

node createNode();

bool hasNext(node current);

node getDictionaryByKey(node head, unsigned char key);

node getLastNode(node head);

unsigned long long printDictionary(node head);

node insertionSort(node head);

node addNode(node head, unsigned char key, unsigned long long value);

node incrementValue(node head, unsigned char chiave);

unsigned long long getTotFrequency(node head);