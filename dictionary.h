#include <stdlib.h>
#include <stdbool.h>

struct Dictionary{
    char key;
    int value;
    struct Dictionary *next;
};

typedef struct Dictionary *node;

node createNode();

bool hasNext(node current);

node getDictionaryByKey(node head, char key);

node getLastNode(node head);

int printDictionary(node head);

node insertionSort(node head);

node addNode(node head, char key, int value);

node incrementValue(node head, char chiave);

int getTotFrequency(node head);