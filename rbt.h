#ifndef RBT
#define RBT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "list.h"
#include "functions.h"



// typedef struct rbt_node{
// 	//data of node	
// 	list_node *listPtr; //pointer to patients-list
// 	date data_date; //date of node 
// 	// int data; //data to check if rbt works correct 	

// 	int color; //1 if red 
// 	struct rbt_node *left, *right, *parent;
// } rb_node;


rb_node *newRBTNode(date * my_date);
void freeRBTNode(rb_node *);
void free_rb(rb_node *root);

void inorder(rb_node *root);
void RotateLeft(rb_node **root, rb_node *x);
// rb_node* search(rb_node* root, int k);
void RotateRight(rb_node **root, rb_node *y);
void FixViolation(rb_node **root,rb_node *z);
void insert(rb_node **root, rb_node *z);
rb_node * minValueNode(rb_node* node);


void print_rb_node(rb_node *node);
// int lookup_rb_tree(rb_node *, int);


#endif
