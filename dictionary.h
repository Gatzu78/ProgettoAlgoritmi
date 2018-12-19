#include <stdlib.h>
#include <stdbool.h>

struct Dictionary{
    unsigned char key;
    long value;
    struct Dictionary *next;
};

typedef struct Dictionary *node;

node createNode();

bool hasNext(node current);

node getDictionaryByKey(node head, unsigned char key);

node getLastNode(node head);

long printDictionary(node head);

node insertionSort(node head);

node addNode(node head, unsigned char key, long value);

node incrementValue(node head, unsigned char chiave);

long getTotFrequency(node head);