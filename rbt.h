#ifndef RBT
#define RBT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "list.h"
#include "functions.h"



rb_node *newRBTNode(date * my_date);

void freeRBTNode(rb_node *);
void free_rb(rb_node *root);

void inorder(rb_node *root);
void RotateLeft(rb_node **root, rb_node *x);
void RotateRight(rb_node **root, rb_node *y);
void FixViolation(rb_node **root,rb_node *z);
void insert(rb_node **root, rb_node *z);
rb_node *minValueNode(rb_node* node);

list_node *rbtSearch(rb_node *root, date my_date);

void print_rb_node(rb_node *node);
// int lookup_rb_tree(rb_node *, int);


#endif
