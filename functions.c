#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "functions.h"
#include "structs.h"
#include "rbt.h"


void print_entry(entry *my_entry){
	printf("==========PRINTING ENTRY===========\n");
	printf("id: %s\n", my_entry->recordID);
	printf("FirstName: %s\n", my_entry->patientFirstName);
	printf("LastName: %s\n", my_entry->patientLastName);
	printf("diseaseID: %s\n", my_entry->diseaseID);
	printf("country: %s\n", my_entry->country);
	printf("entry Date: %d/%d/%d \n", my_entry->entryDate.day, my_entry->entryDate.month, my_entry->entryDate.year);
	printf("exit Date: %d/%d/%d \n", my_entry->exitDate.day, my_entry->exitDate.month, my_entry->exitDate.year);
	printf("\n");
}


entry* line_to_entry(char *line){
	entry *my_entry = malloc(sizeof(entry));
	// printf("%s\n", line);
	
	char *token = strtok(line," ");
    if (token == NULL)
    	return NULL;
    my_entry->recordID = malloc(sizeof (char) * (strlen(token)+1));
    strcpy(my_entry->recordID, token);
    
    token = strtok(NULL, " ");
    if (token == NULL)
    	return NULL;
    my_entry->patientFirstName = malloc(sizeof (char) * (strlen(token)+1));
    strcpy(my_entry->patientFirstName, token);

    token = strtok(NULL, " ");
    if (token == NULL)
    	return NULL;
    my_entry->patientLastName = malloc(sizeof (char) * (strlen(token)+1));
    strcpy(my_entry->patientLastName, token);

    token = strtok(NULL, " ");
    if (token == NULL)
    	return NULL;
    my_entry->diseaseID = malloc(sizeof (char) * (strlen(token)+1));
    strcpy(my_entry->diseaseID, token);

    token = strtok(NULL, " ");
    if (token == NULL)
    	return NULL;
    my_entry->country = malloc(sizeof (char) * (strlen(token)+1));
    strcpy(my_entry->country, token);

    token = strtok(NULL, "-");
    if (token == NULL)
    	return NULL;
    my_entry->entryDate.day = atoi(token);

    token = strtok(NULL, "-");
    if (token == NULL)
    	return NULL;
    my_entry->entryDate.month = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    	return NULL;
    my_entry->entryDate.year = atoi(token);

	// printf("entry Date: %d/%d/%d \n", my_entry->entryDate.day, my_entry->entryDate.month, my_entry->entryDate.year);


    // token = strtok(NULL, " ");
    token = strtok(NULL, "-");
    // if (token == NULL)
    	// return NULL;
    // printf("token _%s_\n", token); 
    // if (strlen(token) == 3)
    if (token == NULL)
    {
    	my_entry->exitDate.day = 1;
    	my_entry->exitDate.month = 1;
    	my_entry->exitDate.year = 1;
    }
    else 
    {
	    my_entry->exitDate.day = atoi(token);
	    
	    token = strtok(NULL,"-");
	    if (token == NULL)
    		return NULL;
	    // printf("%s\n", token);
	    my_entry->exitDate.month = atoi(token);

	    token = strtok(NULL," ");
	    if (token == NULL)
    		return NULL;
	    // printf("%s\n", token);
	    my_entry->exitDate.year = atoi(token);

    }
   	// print_entry(my_entry);
    return my_entry; 
}



//hash functions take id and prime number 
//and return the hashed value
int hash1(char *str, int factor){
	int size = strlen(str);
	int hashValue = 0;
	for(int i = 0; i < size; i++){
		//count the sum of ASCII codes of the characters of the string multiplied by 2 
		hashValue = 2*(hashValue + (int)str[i]);
	}
	// printf("h1: %d\n",hashValue%factor);
	return hashValue%factor; 
}



int hash2(char *str, int factor){
	int size = strlen(str);
	int hashValue = 0;
	for (int i = 0; i < size; i++){
		//count the sum of ASCII codes of the characters of the string multiplied by their position in the string 
		hashValue += i* (int)str[i];
	}
		// printf("h2: %d\n",hashValue%factor);
	return hashValue%factor;
}

void set_date(int mday, int mmonth, int myear, date *my_date){
	my_date->day = mday;
	my_date->month = mmonth;
	my_date->year = myear;
}



//returns 1 if date1 is earlier than date2 
//returns 0 if date1 == date2
//returns -1 if date1 > date2
int earlier(date *date1, date *date2){ //call this with &
	if (date2->year == 1 && date2->month == 1 && date2->day == 1)
		return 1;
		
	

	if (date1->year < date2->year)
		return 1;
	else if (date1->year == date2->year && date1->month < date2->month)
		return 1;
	else if (date1->year == date2->year && date1->month == date2->month && date1->day < date2->day)
		return 1;
	else if (date1->year == date2->year && date1->month == date2->month && date1->day == date2->day)
		return 0;

	return -1; //date1 is after date2
}


bucket * initialize_bucket(int cap){
	bucket * new_bucket = malloc(sizeof(bucket));
	new_bucket->next = NULL;
	new_bucket->currentNumberOfEntries = 0;
	new_bucket->entries = malloc(sizeof(bucket_entry) * cap);
	for (int i = 0; i < cap; i++)
	{
		new_bucket->entries[i].root = NULL;
	}
	return new_bucket;
}


rb_node **diseaseIDexists(bucket *my_bucket, char *my_diseaseID){
	bucket * last_bucket = my_bucket;
	while (last_bucket != NULL){
		for (int k = 0; k < last_bucket->currentNumberOfEntries; k++)
		{
	 		if (strcmp(last_bucket->entries[k].nameOfdisease,my_diseaseID) == 0)
	 		{
	 			// printf("%s already exists\n", my_diseaseID);
	 			return &last_bucket->entries[k].root; 
	 		}
		}
		last_bucket = last_bucket->next;
	}
	// printf("%s not exists\n", my_diseaseID);
	return NULL;
}


//insert entry to last bucket of buckets-list
//if bucket is full allocate a new bucket and connect it with the last bucket of the buckets-list
void insert_entry_to_bucket(bucket *last_bucket, char *diseaseID, list_node *new_node, int capacity){
	if (last_bucket->currentNumberOfEntries < capacity) //fits in current bucket 
	{	
		//field 1
		last_bucket->entries[last_bucket->currentNumberOfEntries].nameOfdisease = malloc(sizeof(char) * (strlen(diseaseID) + 1));
		strcpy(last_bucket->entries[last_bucket->currentNumberOfEntries].nameOfdisease, diseaseID);
		//field 2 
		rb_node * new_tree_node= newRBTNode(&new_node->data->entryDate);
		insert(&last_bucket->entries[last_bucket->currentNumberOfEntries].root, new_tree_node);
		//deikti red black komvou = new_node
		last_bucket->entries[last_bucket->currentNumberOfEntries].root->listPtr = new_node;

		last_bucket->currentNumberOfEntries++;
	}	
	else //needs a new bucket
	{
		bucket *new_bucket = initialize_bucket(capacity);
		//field 1
		new_bucket->entries[0].nameOfdisease = malloc(sizeof(char) * (strlen(diseaseID) + 1));
		strcpy(new_bucket->entries[0].nameOfdisease, diseaseID);
		//field 2
		rb_node * new_tree_node= newRBTNode(&new_node->data->entryDate);
		insert(&new_bucket->entries[0].root, new_tree_node);
		//deikti red black komvou = new_node
		new_bucket->entries[0].root->listPtr = new_node;

		new_bucket->currentNumberOfEntries++;
		last_bucket->next = new_bucket;	
	}
}


void insert_to_hash(bucket **diseaseHashTable, int diseaseHashNum, list_node *new_node, int capacity){
	int hashValue = hash1(new_node->data->diseaseID, diseaseHashNum);
	// printf("hash value: %d\n", hashValue);
	if (diseaseHashTable[hashValue] == NULL)
	{
		diseaseHashTable[hashValue] = initialize_bucket(capacity);
		//set first entry of bucket
		//field 1
		diseaseHashTable[hashValue]->entries[0].nameOfdisease = malloc(sizeof(char) * (strlen(new_node->data->diseaseID) + 1));
		strcpy(diseaseHashTable[hashValue]->entries[0].nameOfdisease, new_node->data->diseaseID);
		//field 2
		rb_node * new_tree_node= newRBTNode(&new_node->data->entryDate);
		insert(&diseaseHashTable[hashValue]->entries[0].root, new_tree_node);
		//deikti red black komvou = new_node
		diseaseHashTable[hashValue]->entries[0].root->listPtr = new_node;
		diseaseHashTable[hashValue]->currentNumberOfEntries = 1;
	}
	else
	{	
		rb_node ** root = diseaseIDexists(diseaseHashTable[hashValue], new_node->data->diseaseID);
		if (root == NULL)
		{
			bucket *last_bucket = diseaseHashTable[hashValue]; 
			while (last_bucket->next != NULL) //get last bucket 
				last_bucket = last_bucket->next;
			insert_entry_to_bucket(last_bucket, new_node->data->diseaseID, new_node, capacity);
		}
		else 
		{
			rb_node * new_tree_node= newRBTNode(&new_node->data->entryDate);
			insert(root, new_tree_node);
		}
	}
}

void free_bucket(bucket *buc){
	for (int j = 0; j < buc->currentNumberOfEntries; j++)
	{
	 	// printf("name of entry %d: %s\n", j, buc->entries[j].nameOfdisease);
	 	free(buc->entries[j].nameOfdisease); 	
	 	free_rb(buc->entries[j].root);
	}

	free(buc->entries);
	free(buc);
}

void free_hash(bucket **diseaseHashTable, int diseaseHashNum){

	for (int i = 0; i < diseaseHashNum; i++)
	{
		// printf("%p\n", diseaseHashTable[i]);
		if (diseaseHashTable[i] != NULL)
		{
			bucket *head = diseaseHashTable[i];
			bucket *tmp;
			while (head != NULL){
				tmp = head;
				head = head->next;
				free_bucket(tmp);
			}
			// free(diseaseHashTable[i]);
		}		
	}
	free(diseaseHashTable);
}