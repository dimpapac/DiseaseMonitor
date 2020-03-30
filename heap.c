#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

// typedef struct heapnode{
// 	int data;
// 	char *stringData;	

// 	struct heapnode *left, *right, *parent, *prevTail; 
// }heap_node;

// typedef struct heap{
// 	heap_node *root, *tail;
// 	int size;
// }heap;

// typedef struct heap_list_node{
// 	heap_node *hnode;
// 	struct heap_list_node *next;
// }heapListNode;

// typedef struct list{
// 	heapListNode **heapListHead;
// }HeapList;

//////////////////heap list/////////////////////

heapListNode * newHeapListNode(int sum, char *name){
	// printf("newHeapListNode\n");
	heapListNode *newhln = malloc(sizeof(heapListNode));
	newhln->next = NULL;
	newhln->hnode = newHeapNode(sum, name);

	return newhln;
}



void updateHeapList(heapListNode **head, char *string, int k){

	if (*head == NULL) //list is empty 
	{
		*head = newHeapListNode(k, string); 
		return;
	}
	else
	{
		//first node
		heapListNode *last = *head;
		if (strcmp(last->hnode->stringData, string) == 0) //string already exists
		{
			// printf("%s already exists\n", string);
			last->hnode->data = last->hnode->data + k;
			return;
		}
		//traverse till the last node 
		while (last->next != NULL){
			last = last->next; 
			if (strcmp(last->hnode->stringData, string) == 0) //string already exists
			{
				// printf("%s already exists\n", string);
				last->hnode->data = last->hnode->data + k;
				return;
			}
		}

		// printf("%s doesnt exist\n", string);
		last->next = newHeapListNode(k, string); //new stringData 	
		return;
	}

	// return new_node;
}




void printHeapList(heapListNode *head){

	while(head != NULL){
		printf("%s %d\n", head->hnode->stringData, head->hnode->data);
		head = head->next;
	}
}


void freeHeapList(heapListNode *head){
	heapListNode *tmp = NULL;
	while (head != NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
}



///////////////////////////////////////////////
heap_node *newHeapNode(int k, char *name){
	// printf("newHeapNode\n");
	heap_node * node = malloc(sizeof(heap_node));
	node->data = k;
	node->stringData = malloc(sizeof(char)* (strlen(name) + 1));
	strcpy(node->stringData, name);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->prevTail = NULL;

	return node;
}

heap *newHeap(){
	heap *my_heap = malloc(sizeof(heap));
	my_heap->root = my_heap->tail = NULL;
	my_heap->size = 0;
	return my_heap;
}


void swapNodeData(heap_node *a, heap_node *b){
	int temp = a->data;
	a->data = b->data;
	b->data = temp;

	char *ctemp = a->stringData;
	a->stringData = b->stringData;
	b->stringData =ctemp;
}

void maxHeapify(heap_node *node){
	if(node->parent != NULL){
		if(node->parent->data < node->data){
		swapNodeData(node->parent, node);
		maxHeapify(node->parent);
		}
	}
}


void revMaxHeapify(heap_node *node){
	if(node == NULL || node->left == NULL) //no children
		return;
	heap_node *max = node->left;
	if(node->right != NULL && max->data < node->right->data){
		max = node->right;
	}
	if(max->data > node->data){
		swapNodeData(node, max);
		revMaxHeapify(max);
	}
}




void setTail(heap *my_heap, heap_node *node){
	/*If we reach this stage that means a level is completely filled
	and we need to proceed to the next level by going to the extreme left.*/
	if(node->parent == NULL){
		my_heap->tail = node;
		while(my_heap->tail->left != NULL){
			my_heap->tail = my_heap->tail->left;
		}
	}
	/*If current node is the left node, go to the right node and
	proceed left from there to reach the left most node.*/
	else if(node->parent->left == node){
		my_heap->tail = node->parent->right;
		while(my_heap->tail->left != NULL){
			my_heap->tail = my_heap->tail->left;
		}
	}
	else if(node->parent->right == node){
		setTail(my_heap, node->parent);
	}
}


void insertHeap(heap *my_heap, heap_node *new_node){
	if(my_heap->root == NULL){
		my_heap->root = new_node;
		my_heap->tail = my_heap->root;
	}
	else if(my_heap->tail->left == NULL){
		my_heap->tail->left = new_node;
		my_heap->tail->left->parent = my_heap->tail;
		maxHeapify(my_heap->tail->left);
	}else {
		my_heap->tail->right = new_node;
		my_heap->tail->right->parent = my_heap->tail;
		maxHeapify(my_heap->tail->right);
		heap_node *prevTail = my_heap->tail;
		setTail(my_heap, my_heap->tail);
		my_heap->tail->prevTail = prevTail;
	}
	my_heap->size++;
}


void freeHeapNode(heap_node *node){
	// printf("freeHeapNode\n");
	free(node->stringData);
	free(node);
}


void deleteRoot(heap *my_heap){
	if(my_heap->root == NULL) {
		// printf("maxHeap is empty\n");
		return;
	}
	
	if (my_heap->size == 1)
	{
		// printf("last node\n");
		//free last node
		freeHeapNode(my_heap->root);
		my_heap->root = NULL;
		my_heap->tail = NULL;
		my_heap->size = 0;
		return;

	}
	
	if (my_heap->tail->right != NULL){
		swapNodeData(my_heap->tail->right, my_heap->root);
		freeHeapNode(my_heap->tail->right); //free
		my_heap->tail->right = NULL; 
		revMaxHeapify(my_heap->root);
	}
	else if(my_heap->tail->left != NULL) {
		swapNodeData(my_heap->tail->left, my_heap->root);
		freeHeapNode(my_heap->tail->left); //free
		my_heap->tail->left = NULL; 
		revMaxHeapify(my_heap->root);
	}
	else {
		my_heap->tail = my_heap->tail->prevTail;
		deleteRoot(my_heap);
		my_heap->size++;
	}
	my_heap->size--;
	
	
}


//this function was used to check the rotations 
void inorderHeap(heap_node *root) {
    if (root != NULL) { 
        inorderHeap(root->left); 
        printf("%d %s\n", root->data, root->stringData); 
        if (root->parent != NULL){
        	printf("parent of %d %s----->%d %s\n", root->data, root->stringData, root->parent->data, root->parent->stringData);
        }
        
        inorderHeap(root->right); 
    } 
} 


void printHeapNode(heap_node * node){
	if (node == NULL) return;
	printf("data of node: %d %s\n", node->data, node->stringData);
	printf("parent: %p\n", node->parent);
	printf("left: %p\n", node->left);
	printf("right: %p\n", node->right);

}

void free_heap(heap *my_heap){
	free(my_heap);
}

