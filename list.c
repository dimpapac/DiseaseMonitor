#include "list.h"
#include "functions.h"
#include "structs.h"
#include "heap.h"


list_node * sortedInsert(list_node **head, entry *new_entry){
	list_node *current;

	list_node* new_node = malloc(sizeof(list_node));
	new_node->data = new_entry;
	new_node->next = NULL;

	// if (*head == NULL || (*head)->data->date >= new_entry->data->date) 
	if (*head == NULL || earlier( &((*head)->data->entryDate), &(new_node->data->entryDate)) == -1) // >
    { 
        new_node->next = *head; 
        *head = new_node; 
    } 
    else if (earlier( &((*head)->data->entryDate), &(new_node->data->entryDate)) == 0)
    {
    	new_node->next = (*head)->next;
    	(*head)->next = new_node;
    }
    else
    { 
        current = *head; 
        // while (current->next!=NULL && current->next->data < new_node->data) 
        while (current->next!=NULL && earlier(&(current->next->data->entryDate), &(new_node->data->entryDate)) != -1) //<=
        { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 

   	return new_node;
}


list_node* append(list_node **head, entry *new_entry){

	list_node* new_node = malloc(sizeof(list_node));
	list_node* last = *head;
	new_node->data = new_entry;
	new_node->next = NULL;

	if (*head == NULL) //list is empty 
	{
		*head = new_node; 
	}
	else
	{
		//traverse till the last node 
		while (last->next != NULL)
			last = last->next; 

		last->next = new_node; 		
	}

	return new_node;
}

//return 1 if id found in list
//else 0
list_node *search(list_node* head, char *id) 
{ 
    list_node* current = head;  // Initialize current 
    while (current != NULL) 
    { 
        if (strcmp(current->data->recordID, id) == 0)        	
            return current; 
        current = current->next; 
    } 
    return NULL; 
}


void print_list(list_node *head){
	// if (head == NULL)
	// 	return;

	while(head != NULL){
		// printf("kalispera apo print list\n");
		print_entry(head->data);
		head = head->next;
	}
}

void free_node(list_node *node){
	free(node->data->recordID);
	free(node->data->patientFirstName);
	free(node->data->patientLastName);
	free(node->data->diseaseID);
	free(node->data->country);
	free(node->data);
	free(node);
}

void free_list(list_node *head){
	list_node *tmp = NULL;
	while (head != NULL){
		tmp = head;
		head = head->next;
		free_node(tmp);
		// free(tmp);
	}
}