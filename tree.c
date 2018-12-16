//
// Created by Simone on 08.10.18.
//


#include "tree.h"



nodeHuffman hfTable[256];
char headerFile[256];
int currentIndex = 0;

nodeTree createTree(char word, float val){
    nodeTree temp;
    temp = (nodeTree)malloc(sizeof(struct Tree));
    temp->left = NULL;
    temp->right = NULL;
    temp->word = word;
    temp->val = val;
    return temp;
}

nodeTree mergeTree(nodeTree left, nodeTree right){
    nodeTree temp;
    temp = (nodeTree)malloc(sizeof(struct Tree));
    temp->left = left;
    temp->right = right;
    temp->val = left->val+right->val;
    return temp;
}

nodeTree getLeftChild(nodeTree nt){
    return nt->left;
}

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

nodeFrequency createFrequencyNode(nodeTree tree){
    nodeFrequency temp = (nodeFrequency)malloc(sizeof(struct FrequencyList));
    temp->tree = tree;
    temp->next = NULL;
    return temp;
}

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

int populateTree(node dic){
    dic = insertionSort(dic);
    int tot_freq = getTotFrequency(dic);
    printDictionary(dic);
    printf("Tot Frequency: %d\n", tot_freq);

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
    int arr[256];
    printTable(headFrequency->tree, arr ,0);

    //printHuffmanTable(hfTable, 255);
    sortHuffmanTable(hfTable);
    createHeader(hfTable,headerFile);

    printHuffmanTable(hfTable, 256);
    return 0;
}

void compressString(char *file){
    FILE *fp = NULL;
    fp = openFile(fp,"test2", true);


    for(int h = 0;h<256;h++){
        writeByte(fp, headerFile[h]);
    }

    char c;
    int cnt = 0;
    unsigned char mask = 0x00;
    while ((c = *file++) != '\0') {
        nodeHuffman dumHuffman = getHuffmanNodeByKey(hfTable, 256, c);
        char *code = dumHuffman->coded;
        char b;
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

    //scrito quanto bisogna togliere al byte finale in un byte che terminerà il file compresso
    char bitToRemove = (8-cnt)&0x15;
    writeByte(fp,bitToRemove);


    closeFile(fp);
}

void buildTree(){
    FILE *fp = NULL;
    fp = readFile(fp,"test2");

    char headerFile[256];
    long cnt;

    fseek(fp, 0, SEEK_END);
    cnt = ftell(fp);
    rewind(fp);


    char *buffer;
    buffer = (char *)malloc((cnt+1)*sizeof(char));
    fread(buffer, cnt, 1, fp);

    for (int i = 0; i < 256; ++i) {
        headerFile [i] = buffer[i];
    }

    char lastByte =  buffer[cnt-1];
    fclose(fp);


}