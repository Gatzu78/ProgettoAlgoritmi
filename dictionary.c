#include <printf.h>
#include "dictionary.h"

node createNode(){
    node temp;
    temp = (node)malloc(sizeof(struct Dictionary));
    temp->next = NULL;
    return temp;
}

bool hasNext(node current){
    if(current == NULL || current->next==NULL){
        return false;
    }
    return true;
}

node getDictionaryByKey(node head, unsigned char key){
    if(head==NULL){
        return NULL;
    }
    node diz = head;
    while(hasNext(diz)){
        if(diz->key == key){ //trovata la chiave
            return diz;
        }
        diz = diz->next;
    }

    if(diz->key == key){ //trovata la chiave
        return diz;
    } else{
        return NULL;
    }
}

node getLastNode(node head){
    if(head==NULL){
        return head;
    }
    while(hasNext(head)){
        head=head->next;
    }
    return head;
}

long printDictionary(node head){
    if(head==NULL){
        return 1;
    }
    while(head != NULL){
        printf("%c-%d\t",head->key,head->value);
        head=head->next;
    }
    printf("\n");
    return 0;
}


/**
 * http://www.zentut.com/c-tutorial/c-linked-list/#Sort_a_linked_list_using_insertion_sort questo algoritmo è stato preso da questo
 * sito e riadattato al caso attuale e commentato
 * @param head
 * @return
 */
node insertionSort(node head){

    node x, y, e; //salvo i 3 puntatori

    x = head; //viene salvato il valore attuale del nodo iniziale
    head = NULL; //resetto il mio nodo che preparo poi per essere ritornato

    while(x != NULL) //continuo a ciclare la lista, fin quando non arrivo all'ultimo elemento
    {
        e = x; //salvo il valore attuale
        x = x->next; //incremento il puntatore della lista al prossimo elemento
        if (head != NULL) //se il mio nodo di ritorno
        {
            if(e->value > head->value)
            {
                y = head;
                while ((y->next != NULL) && (e->value > y->next->value))
                {
                    y = y->next;
                }
                e->next = y->next;
                y->next = e;
            }
            else
            {
                e->next = head;
                head = e ;
            }
        }
        else
        {
            e->next = NULL;
            head = e ;
        }
    }
    return head;
}

node addNode(node head, unsigned char key, long value){
    node temp = createNode();
    node last = getLastNode(head);
    temp->key = key;
    temp->value = value;
    temp->next=NULL;
    if(last==NULL){
        return temp;
    }else{
        last->next=temp;
    }
    return head;
}

node incrementValue(node head, unsigned char chiave){
    node diz = getDictionaryByKey(head, chiave);
    if(diz != NULL){
        diz->value=diz->value+1;
    }else{
        head = addNode(head, chiave, 1);
    }
    return head;
}

long getTotFrequency(node head){
    int current = 0;
    while(head!=NULL){
        current += head->value;
        head=head->next;
    }
    return current;
}