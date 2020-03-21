#ifndef LIST
#define LIST

#include "structs.h"

// typedef struct li_node {
// 	entry *data;
// 	struct li_node *next;
// } list_node;

list_node * sortedInsert(list_node **head, entry *new_entry);
list_node* append(list_node** head, entry* new_entry);

int search(list_node* head, char *id) ;
void print_list(list_node *head);
void free_list(list_node *head);



#endif