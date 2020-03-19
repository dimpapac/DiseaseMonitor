#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"
#include "list.h"
#include "rbt.h"


int main(int argc, char *argv[])
{

	if (argc != 9)
	{
		printf("Wrong number of parameters\n");
		return -1;
	}

	char *inputfile;
	FILE *input;
	int diseaseHashNum, countryHashNum, bucketSize;

	for (int i = 1; i < argc; i+=2)
	{
		if (strcmp(argv[i], "-p") == 0)
		{
			inputfile = malloc((strlen(argv[i+1]) + 1) * sizeof(char));
			strcpy(inputfile, argv[i+1]);
		}
		else if (strcmp(argv[i], "-h1") == 0)
		{
			diseaseHashNum = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-h2") == 0)
		{	
			countryHashNum = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-b") == 0)
		{
			bucketSize = atoi(argv[i+1]);
		}
	}

	input = fopen (inputfile,"r");
	if (input == NULL){
		printf("Cannot open file\n");
		return -1;
	}

	// // check number of entries
	// fseek (input , 0 , SEEK_END);
	// lSize = ftell (input);
	// rewind (input);
	// numOfrecords = (int) lSize/sizeof(rec);

	printf("inputfile %s\n", inputfile);
	printf("diseaseHashNum %d\n", diseaseHashNum);
	printf("countryHashNum %d\n", countryHashNum);
	printf("bucketsize %d bytes\n", bucketSize);

	//initialize hash tables 
	bucket **diseaseHashTable = malloc(sizeof(bucket *) * diseaseHashNum);
	bucket **countryHashTable = malloc(sizeof(bucket *) * countryHashNum);

	for (int i = 0; i < diseaseHashNum; i++)
		diseaseHashTable[i] = NULL;

	for (int i = 0; i < countryHashNum; i++)
		countryHashTable[i] = NULL;

	// int capacity; // capacity = (bucketSize - sizeof(struct b*) - sizeof(int) )/ sizeof(bucket_entry)

	char *line = NULL;
	size_t len = 0;
	ssize_t lineSize;
	
	int capacity = (bucketSize - sizeof(struct b*) - sizeof(int) )/ sizeof(bucket_entry);
	printf("sizeof bucket entry %lu\n", sizeof(bucket_entry));

	list_node *head = NULL; // head of list 
		
	while ((lineSize = getline(&line, &len, input)) != -1) {
		// printf("lineSize = %lu    len = %lu\n", lineSize, len);
		line[lineSize - 1] = '\0';
		// printf("\n");
		// printf("%s\n", line);
		entry* new_entry = line_to_entry(line);
		// print_entry(new_entry);
		// printf("%d\n", earlier(&new_entry->entryDate, &new_entry->exitDate));
		if (earlier(&new_entry->entryDate, &new_entry->exitDate) == -1)
		{
			printf("Wrong dates\n");
			//free entry
			free(new_entry->recordID);
			free(new_entry->patientFirstName);
			free(new_entry->patientLastName);
			free(new_entry->diseaseID);
			free(new_entry->country);
			free(new_entry);
			free(line);
			line = NULL;
			continue;
		}

		// list_node *new_node = append(&head, new_entry);
		list_node *new_node = sortedInsert(&head, new_entry);
		
		insert_to_hash(diseaseHashTable, diseaseHashNum, new_node, capacity); 

		free(line);
		line = NULL;
	}


	// print_list(head);

	printf("capacity: %d\n", capacity);

	// print entries of bucket	
	printf("%d\n", diseaseHashNum);
	for (int i = 0; i < diseaseHashNum; i++)
	{
		printf("%p\n", diseaseHashTable[i]);
		if (diseaseHashTable[i] != NULL)
		{
			printf("cur entries: %d for bucket No: %d\n", diseaseHashTable[i]->currentNumberOfEntries, i);
			for (int j = 0; j < diseaseHashTable[i]->currentNumberOfEntries; j++)
			{
			 	printf("name of entry %d: %s root: %p\n", j, diseaseHashTable[i]->entries[j].nameOfdisease, diseaseHashTable[i]->entries[j].root);
			 	if (diseaseHashTable[i]->entries[j].root != NULL)
			 	{
			 		inorder(diseaseHashTable[i]->entries[j].root);
			 	}
			} 
			bucket * last_bucket = diseaseHashTable[i]->next;
			while (last_bucket != NULL){
				for (int k = 0; k < last_bucket->currentNumberOfEntries; k++)
				{
			 		printf("name of entry %dk: %s root: %p\n", k, last_bucket->entries[k].nameOfdisease, last_bucket->entries[k].root);
			 		if (last_bucket->entries[k].root != NULL)
			 		{
			 			inorder(last_bucket->entries[k].root);
			 		}
				}
				last_bucket = last_bucket->next; 
			}
		}
	}




	free_hash(diseaseHashTable, diseaseHashNum);
	free_hash(countryHashTable, countryHashNum);

	free_list(head);
	free(line);
	line = NULL;

	fclose(input); //close file
	free(inputfile);
	

	
	return 0;	
}