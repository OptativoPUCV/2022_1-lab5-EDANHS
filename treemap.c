#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *newMap = (TreeMap*) malloc(sizeof(TreeMap));
    if(newMap == NULL) exit(1);
    newMap->root = NULL;
    newMap->current = NULL;
    newMap->lower_than = (lower_than);
    //new->lower_than = lower_than;
    return newMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(searchTreeMap(tree,key) != NULL) return;
    TreeNode *newNode = createTreeNode(key,value);
    if(tree->lower_than(key,tree->current->pair->key)==1){
        tree->current->left = newNode;
    }else{
        tree->current->right = newNode;
    }
    newNode->parent = tree->current;
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(node->left == NULL && node->right == NULL){
        if(node->parent->left == node){
            node->parent->left = NULL;
        }else{
            node->parent->right = NULL;
        }
        free(node);
        return;
    }
    if((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL)){
        if(node->parent->right != NULL){
            if(node->right != NULL){
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            else{
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        if(node->parent->left != NULL){
            if(node->right != NULL){
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            else{
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
        }
        free(node);
        return;
    }
    TreeNode *aux = minimum(node->right);
    node->pair = aux->pair;
    removeNode(tree,aux);
    free(node);

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    while (aux != NULL){
        tree->current = aux;
        if((tree->lower_than(key,aux->pair->key))==1){
            aux = aux->left;
        } else if ((tree->lower_than(aux->pair->key,key))==1){
            aux = aux->right;
        } else{
            return aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    TreeNode *ub_node = tree->root;
    while (aux != NULL){
        tree->current = aux;
        if((tree->lower_than(key,aux->pair->key))==1){
            aux = aux->left;
            if(tree->lower_than(key,aux->pair->key)==1 && aux != NULL){
                ub_node = aux;
            }
        } else if ((tree->lower_than(aux->pair->key,key))==1){
            aux = aux->right;
            if(tree->lower_than(key,aux->pair->key)==1 && aux != NULL){
                ub_node = aux;
            }
        } else{
            return aux->pair;
        }
    }
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *aux = minimum(tree->root);
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = NULL;
    if(tree->current->right != NULL){
        aux = minimum(tree->current->right);
        tree->current = aux;
        //printf("key1: %d\n",*(int*)aux->pair->key);
        return aux->pair;
    }
    aux = tree->current;
    while(aux->parent!=NULL){
        if(tree->lower_than(aux->pair->key,aux->parent->pair->key)==1){
            aux=aux->parent;
            tree->current = aux;
            return aux->pair;
        }
        aux = aux->parent;
    }
    return NULL;
}
