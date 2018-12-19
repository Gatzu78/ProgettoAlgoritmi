//
// Created by Simone on 08.10.18.
//


#include "tree.h"



nodeHuffman hfTable[256];
unsigned char headerFile[256];
unsigned long currentIndex = 0;

nodeTree createTree(unsigned char word, unsigned long val){
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
    int arr[256] = {0};
    printTable(headFrequency->tree, arr ,0);

    sortHuffmanTable(hfTable);
    //printHuffmanTable(hfTable, 255);

    createHeader(hfTable,headerFile);

    printHuffmanTable(hfTable, 256);
    return 0;
}

void compressString(char *file, unsigned long lungfile){
    FILE *fp = NULL;
    fp = fopen("test1.funny", "wb");

//    rewind(fp);

    for(int h = 0;h<256;h++){
        writeByte(fp, headerFile[h]);
    }

    unsigned char c;
    int cnt = 0;
    char mask = 0x00;
    for(int t=0;t<lungfile;t++){//while ((c = *file++) != '\0') {
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

    //scrito quanto bisogna togliere al byte finale in un byte che terminerà il file compresso
    char bitToRemove = (8-cnt)&15;
    writeByte(fp,bitToRemove);


    closeFile(fp);
}

void buildTree(){
    FILE *fp;
    //fp = readFile(fp,"test1");

    fp = fopen("test1.funny", "rb");

    nodeHuffman headerFile[256] = {0};
    unsigned long cnt;

    fseek(fp, 0, SEEK_END);
    cnt = ftell(fp);

    fseek(fp, 0L, SEEK_SET); //rewind(fp);


    char *buffer;
    buffer = (char *)malloc((cnt+1)*sizeof(char));
    fread(buffer, cnt, 1, fp);

    //salvo tutto il dizionario
    int ind = 0;
    for (int i = 0; i < 256; ++i) {
        if(buffer[i]!=0x00){
            int byteLenght = buffer[i];
            char *elementArray = malloc(sizeof(char) * byteLenght);
            for(int j = 0;j<byteLenght;j++){
                elementArray[j] = "0";
            }
            headerFile [ind] = createNodeHuffman((unsigned char)i,elementArray);
            //headerFile [ind] = buffer[i];
            ind = ind+1;
        }
    }

    sortHuffmanTable(headerFile);

    int count = 1;
    long currentByte = 0x00;
    for (int k = 0; k < 256; k++) {
        if(headerFile[k]==NULL){
            break;
        }
        int dumLenght = len(headerFile[k]->coded);
        if(count == 1){
            count = dumLenght;
        }else if(dumLenght > count){
            currentByte = (currentByte+1)<<dumLenght-count;
            count = dumLenght;
        }else{
            currentByte = currentByte+1;
        }
        for(int i = 0;i<dumLenght;i++){
            if(((currentByte>>(dumLenght-i-1))&0x01)==0x00){
                headerFile[k]->coded[i] = '0';
            }else{
                headerFile[k]->coded[i] = '1';
            }
        }
    }

    printHuffmanTable(headerFile,256);

    nodeTree root = createTree(NULL,0);
    for (int k = 0; k < 256; k++) {
        if (headerFile[k] == NULL) {
            break;
        }
        nodeTree  dumRoot = root;
        for(int t = 0; t < len(headerFile[k]->coded);t++){
            if(headerFile[k]->coded[t]=='0'){
                if(dumRoot->left == NULL){
                    dumRoot->left = createTree(NULL,0);
                }
                dumRoot = dumRoot->left;
            }else{
                if(dumRoot->right == NULL){
                    dumRoot->right = createTree(NULL,0);
                }
                dumRoot = dumRoot->right;
            }
        }
        dumRoot->word = headerFile[k]->key;

    }

    //salvo il gap finale dell'ultimo byte
    char lastByte =  buffer[cnt-1];
    nodeTree  dumRoot = root;

    FILE *fd = NULL;
    fd = openFile(fd,"decompresso",true);

    for (long l = 0; l < cnt-257; l++) {
        for (long i = 0; i < 8; i++) {
            if(l == cnt-258 && i+lastByte==8){
                break;
            }
            if(((buffer[256+l]>>(7-i))&0x01)==0x00){
                dumRoot = dumRoot->left;
            }else{
                dumRoot = dumRoot->right;
            }
            if(dumRoot->right == NULL && dumRoot->left == NULL){ //è una foglia
                writeByte(fd,dumRoot->word);
                dumRoot = root;
            }
        }
    }

    fclose(fp);
    fclose(fp);

}