//
// Created by attilio on 15/10/18.
//

#include <string.h>
#include <stdbool.h>
#include "AVL.h"
#include "utils.h"


treeNode *currentTreeRoot;
int currentTreeSize;

// A utility function to get height of the tree
int height(struct treeNode *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
// Usata sul calcolo delle altezze dell'albero, quindi per decidere che rotazioni usare
int max(int a, int b)
{
    return (a > b)? a : b;
}

/*
 * Helper function that allocates a new node with the given key and
 * NULL left and right pointers.
 */
treeNode* newNode(long realOffset,char str[])
{
    treeNode* node = (struct treeNode*)malloc(sizeof(struct treeNode));
    node->realOffset  = realOffset;
    node->str = (char*)malloc(STRSIZE*sizeof(char));
    node->str=strncpy(node->str,str,STRSIZE);
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
treeNode *rightRotate(struct treeNode *y)
{
    treeNode *x = y->left;
    treeNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
treeNode *leftRotate(struct treeNode *x)
{
treeNode *y = x->right;
treeNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(struct treeNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

treeNode* insertNode(struct treeNode *node, long realOffset, char str[])
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL){
        return(newNode(realOffset,str));
    }

    if (strcmp(str,node->str)<0){
        node->left  = insertNode(node->left, realOffset, str);
    } else if (strcmp(str,node->str)>0){
        node->right = insertNode(node->right, realOffset, str);
    } else {// Equal keys update the realOffset
        node->realOffset=realOffset;
        currentTreeSize--;
        return node;
    }

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if ((balance > 1) && (strcmp(str,node->left->str)<0))
        return rightRotate(node);

    // Right Right Case
    if ((balance < -1) && (strcmp(str,node->right->str)>0))
        return leftRotate(node);

    // Left Right Case
    if ((balance > 1) && (strcmp(str,node->left->str)>0))
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if ((balance < -1) && (strcmp(str,node->right->str)<0))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
treeNode * minValueNode(treeNode* node)
{
    treeNode* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
treeNode* deleteNode(treeNode* root, char str[], long checkOffset)
{
    bool deleted=false;

    // STEP 1: PERFORM STANDARD BST DELETE
    // Albero vuoto
    if (root == NULL){
        return root;
    }

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (strcmp(str,root->str)<0)
        root->left = deleteNode(root->left, str,checkOffset);

        // If the key to be deleted is greater than the
        // root's key, then it lies in right subtree
    else if(strcmp(str,root->str)>0)
        root->right = deleteNode(root->right, str,checkOffset);

        // if key is same as root's key, then This is
        // the node to be deleted, check for offset match, otherwise I skip
    else if(checkOffset==root->realOffset)
    {
        deleted=true;
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct treeNode *temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            } else { // One child case
                *root = *temp; // Copy the contents of the non-empty child
            }

            //free(temp->str);
            free(temp);

        } else { // node with two children: Get the inorder

            // successor (smallest in the right subtree)
            struct treeNode* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->str = temp->str;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->str,checkOffset);
        }
    }

    if(deleted==true){
      // valutare cosa mettere qui
      currentTreeSize--;
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Ricerca ricorsiva di un match di stringa.
treeNode* searchMatch(treeNode * node, char str[]){
    if (node==NULL){
        /* Sono arrivato in fondo all'albero, non c'è nessun match */
        return node;
    }
    #ifdef DEBUG

        printf("Stringa passata vale = %s, la Stringa nel nodo attuale dell'albero vale = %s \n",str,node->str);
    #endif
    if (strncmp(str,node->str,strlen(str))==0) {
        /* Match trovato, ritorno il nodo */
        return node;
    } else if (strncmp(str,node->str,strlen(str))<0){
        searchMatch(node->left,str); // cerco a sinistra
    } else if (strncmp(str,node->str,strlen(str))>0){
        searchMatch(node->right,str); // cerco a destra
    }
}