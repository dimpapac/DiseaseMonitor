#ifndef HEAP
#define HEAP

#include "list.h"
#include "functions.h"
#include "structs.h"


heap_node *newHeapNode(int k, char *name);
heap *newHeap();
void swapNodeData(heap_node *a, heap_node *b);
void maxHeapify(heap_node *node);
void revMaxHeapify(heap_node *node);
void setTail(heap *my_heap, heap_node *node);
void insertHeap(heap *my_heap, heap_node *new_node);
void freeHeapNode(heap_node *node);
void deleteRoot(heap *my_heap);
void inorderHeap(heap_node *root) ;
void printHeapNode(heap_node * node);
void free_heap(heap *my_heap);

heapListNode * newHeapListNode(int sum, char *name);
void updateHeapList(heapListNode **head, char *string, int k);

void freeHeapList(heapListNode *head);



#endif