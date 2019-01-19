//
// Created by Simone on 08.10.18.
//


#include "tree.h"



nodeHuffman hfTable[256];
unsigned char headerFile[256];
unsigned long long currentIndex = 0;

/**
 * Questa funzione crea un nuovo sottoalbero, che è nodo di un albero
 */
nodeTree createTree(unsigned char word, unsigned long long val){
    nodeTree temp;
    temp = (nodeTree)malloc(sizeof(struct Tree));
    temp->left = NULL;
    temp->right = NULL;
    temp->word = word;
    temp->val = val;
    return temp;
}

/**
 * Questa funzione collega due alberi (left e right) ritornando il nodo di collegamento
 */
nodeTree mergeTree(nodeTree left, nodeTree right){
    nodeTree temp;
    temp = (nodeTree)malloc(sizeof(struct Tree));
    temp->left = left;
    temp->right = right;
    temp->val = left->val+right->val;
    return temp;
}

/**
 * ritorna il figlio sinistro del nodo passato
 */
nodeTree getLeftChild(nodeTree nt){
    return nt->left;
}

/**
 * ritorna il figlio destro del nodo passato
 */
nodeTree getRightChild(nodeTree nt){
    return nt->right;
}


void showtree(nodeTree nt){
    if(nt!=NULL){
        showtree(getLeftChild(nt));
        if(nt->word!=NULL){
            printf("\t %c - %f \n", nt->word, nt->val);
        }
        showtree(getRightChild(nt));
    }
}

char* printArr(int arr[], int n) {
    char *digit = malloc(sizeof(char) * n);
    int i;
    for (i = 0; i < n; ++i){
        if(arr[i] == 0){
            digit[i] = '0';
        }else{
            digit[i] = '1';
        }
    }
    return digit;
}

/**
 * Questa funzione serve per poter costruire l'array partendo dall'albero
 */
void printTable(nodeTree nt, int digit[], int size_digit){
    if(nt->left){
        digit[size_digit] = 0;
        printTable(nt->left, digit, size_digit+1);
    }

    if(nt->right){
        digit[size_digit] = 1;
        printTable(nt->right, digit, size_digit+1);
    }

    if (!nt->left && !nt->right) {
        hfTable[currentIndex] = createNodeHuffman(nt->word,printArr(digit, size_digit));
        currentIndex = currentIndex+1;

    }
}

/*
 * crea un nodo base per l'albero delle frequenze
 */
nodeFrequency createFrequencyNode(nodeTree tree){
    nodeFrequency temp = (nodeFrequency)malloc(sizeof(struct FrequencyList));
    temp->tree = tree;
    temp->next = NULL;
    return temp;
}

/**
 * aggiunge il nodo di frequenza alla fine della lista
 */
nodeFrequency addFrequencyNode(nodeFrequency head, nodeTree tree){
    nodeFrequency temp = head;
    if(temp==NULL){
        head =  createFrequencyNode(tree);
        return head;
    }
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next = createFrequencyNode(tree);
    return head;
}

/**
 * rimuove il nodo di frequenza dalla lista
 */
nodeFrequency removeFrequency(nodeFrequency head, nodeFrequency nf){
    if(head==nf){
        head = head->next;
        return nf;
    }
    nodeFrequency temp_before = head;
    nodeFrequency temp = head->next;

    while(temp!=NULL){
        if(temp==nf){
            temp_before->next = temp->next;
            return temp;
        }
    }
    return NULL;

}

/**
 * con l'ausilio di un inserion sort, questa funzione ordina tutta la lista di frequenze in base alla frequenza
 */
nodeFrequency sortFrequency(nodeFrequency hf){
    nodeFrequency head = hf;
    nodeFrequency temp = hf;

    if(head==NULL || head->next == NULL){
        return head;
    }

    if(head->tree->val < head->next->tree->val){
        return head;
    }

    hf = hf->next;
    head = hf;

    while(head->next!=NULL && head->next->tree->val < temp->tree->val){
        head = head->next;
    }

    if(head->next== NULL){
        temp->next = NULL;
    }else{
        temp->next = head->next;
    }
    head->next = temp;

    return hf;

}

/**
 * Questa funzione, ricevuto come parametro il dizionario, crea l'albero delle frequenze
 *
 */
void populateTree(node dic){
    dic = insertionSort(dic);
    unsigned long long tot_freq = getTotFrequency(dic);

    node temp = dic;

    nodeTree tempTree = createTree(temp->key, temp->value);
    nodeFrequency headFrequency = addFrequencyNode(NULL,tempTree);

    temp = temp->next;
    while(temp!=NULL){
        nodeTree tempTree = createTree(temp->key, temp->value);
        headFrequency = addFrequencyNode(headFrequency,tempTree);
        temp = temp->next;
    }

    while(headFrequency->next != NULL){
        nodeFrequency first = headFrequency;
        nodeFrequency second = removeFrequency(headFrequency,headFrequency->next);

        nodeTree current = mergeTree(first->tree,second->tree);
        headFrequency->tree = current;

        headFrequency = sortFrequency(headFrequency);
    }

    int arr[256] = {0};

    printTable(headFrequency->tree, arr ,0);

    sortHuffmanTable(hfTable);

    createHeader(hfTable,headerFile);
}

/**
 * questa funzione, ricevuto come parametro il file, con l'ausilio delle strutture, genera il file compresso
 */
void compressString(char *file, unsigned long long lungfile, char *nameFile){
    FILE *fp = NULL;
    fp = openFile(fp,nameFile,true);

    for(int h = 0;h<256;h++){
        writeByte(fp, headerFile[h]);
    }

    unsigned char c;
    int cnt = 0;
    char mask = 0x00;
    for(unsigned long long t=0;t<lungfile-1;t++){
        nodeHuffman dumHuffman = getHuffmanNodeByKey(hfTable, 256, file[t]);
        char *code = dumHuffman->coded;
        unsigned char b;
        while ((b = *code++) != '\0') {
            if(b=='0'){
                mask = mask<<1;
            }else{
                mask = mask<<1|0x01;
            }
            cnt = cnt+1;
            if(cnt == 8){
                writeByte(fp,mask);
                cnt = 0;
                mask = 0x00;
            }
        }

    }
    mask = mask<<(8-cnt);
    writeByte(fp, mask);

    char bitToRemove = (8-cnt)&15;
    writeByte(fp,bitToRemove);


    closeFile(fp);
}