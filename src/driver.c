#include "rbtree.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();
    

    // node_t *min, *max;

    rbtree_insert(t, 10);
    rbtree_insert(t, 5);
    rbtree_insert(t, 8);
    rbtree_insert(t, 34);
    rbtree_insert(t, 67);
    rbtree_insert(t, 23);
    rbtree_insert(t, 156);
    rbtree_insert(t, 24);
    rbtree_insert(t, 2);
    rbtree_insert(t, 12);
    rbtree_insert(t, 24);
    rbtree_insert(t, 36);
    rbtree_insert(t, 990);
    rbtree_insert(t, 25);

    rbtree_display(t, t->root);
    printf("\n");

    node_t *p;

    // p = t->root;
    // p = p->left;

    inorder_tree_walk(t, t->root);
    printf("\n");
    // printf("\n");
    p = rbtree_find(t, 10);
    rbtree_erase(t, p);
    rbtree_display(t, t->root);
    printf("\n");

    p = rbtree_find(t, 5);
    rbtree_erase(t, p);
    rbtree_display(t, t->root);
    printf("\n");

    p = rbtree_find(t, 8);
    rbtree_erase(t, p);
    rbtree_display(t, t->root);
    printf("\n");
    // p = rbtree_find(t, 34);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 67);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 23);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 156);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 24);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 2);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 12);
    // rbtree_erase(t, p);
    // p = rbtree_find(t, 24);
    // printf("\np %d color %d\n", p->key, p->color);
    // printf("\nleft %d color %d\n", p->left->key, p->left->color);
    // printf("\nright %d color %d\n", p->right->key, p->right->color);
    // printf("\nrightright %d color%d\n", p->right->right->key, p->right->right->color);
    // //rbtree_erase(t, p);
    // p = rbtree_find(t, 36);
    // rbtree_erase(t, p);
    

    // min = rbtree_min(t);
    // max = rbtree_max(t);

    // printf("min : %d\n", min->key);
    // printf("max : %d\n", max->key);

    // delete_rbtree(t);
    // if(t == NULL) printf("시작");
    
    inorder_tree_walk(t, t->root);
    printf("\n");
}