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

	// printf("inputfile %s\n", inputfile);
	// printf("diseaseHashNum %d\n", diseaseHashNum);
	// printf("countryHashNum %d\n", countryHashNum);
	// printf("bucketsize %d bytes\n", bucketSize);

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
		if (new_entry == NULL)
		{
			printf("problem\n");
			return -1;
		}
		// print_entry(new_entry);
		
		if(search(head,  new_entry->recordID) == 1){
			printf("%s already exists, adios\n", new_entry->recordID);
			free(new_entry->recordID);
			free(new_entry->patientFirstName);
			free(new_entry->patientLastName);
			free(new_entry->diseaseID);
			free(new_entry->country);
			free(new_entry);
			free(line);
			line = NULL;

			free_hash(diseaseHashTable, diseaseHashNum);
			free_hash(countryHashTable, countryHashNum);
			free_list(head);

			fclose(input); //close file
			free(inputfile);
			return -1;
		}
		
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

		
		insert_to_hash(diseaseHashTable, diseaseHashNum, new_node->data->diseaseID, new_node, capacity); 
		insert_to_hash(countryHashTable, countryHashNum, new_node->data->country, new_node, capacity);
		free(line);
		line = NULL;
	}

	// print_list(head);

	// printf("capacity: %d\n", capacity);

	size_t n;
	//command prompt
	char *command = NULL;
	do {
		if (command != NULL)
			free(command);
		command = NULL;
		lineSize = getline(&command, &n, stdin); //get line from stdin
		command[lineSize - 1] = '\0';
		printf("COMMAND: %s\n", command); 
/*1*/	if (strncmp(command, "/globalDiseaseStats", strlen("/globalDiseaseStats")) == 0 || strncmp(command, "gds", strlen("gds")) == 0) {
			// printf("/globalDiseaseStats\n");
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// if (token != NULL) printf("%s\n",token);
			if (token == NULL) //no dates given
			{
				printf("oraios den edwses kamia imerominia\n");
			}
			else
			{
				char * date1 = token; //what 
				printf("%s\n", date1);
				token = strtok(NULL," ");
				if (token == NULL)
					printf("you have to give to dates\n");
				else 
					printf("%s\n",token);
			}

/*2*/	} else if (strncmp(command, "lrb", strlen("lrb")) == 0) {
			// printf("lrb\n");
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// printf("%s\n",token);
			

/*3*/	} else if (strncmp(command, "ins", strlen("ins")) == 0) {
			// printf("ins\n");
			


/*4*/	} else if (strncmp(command, "find", strlen("find")) == 0) {
			// printf("find\n");
			
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// printf("%s\n",token);
			

/*5*/	} else if (strncmp(command, "delete", strlen("delete")) == 0) {
			// printf("delete\n");
			printf("delete not implemented\n");
/*6*/	} else if (strncmp(command, "vote ", strlen("vote ")) == 0) {
			// printf("vote\n");
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// printf("%s\n",token);


/*7*/	} else if (strncmp(command, "load", strlen("load")) == 0) {
			// printf("load\n");
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// printf("%s\n",token);
			
			
		} else if (strcmp(command, "/exit\0") != 0){
			printf("Wrong command try again\n");
		}

	} while(strcmp(command, "/exit\0") != 0);
	if (command != NULL)
		free(command);

	print_hash(diseaseHashTable, diseaseHashNum);
	print_hash(countryHashTable, countryHashNum);



	free_hash(diseaseHashTable, diseaseHashNum);
	free_hash(countryHashTable, countryHashNum);

	free_list(head);
	free(line);
	line = NULL;

	fclose(input); //close file
	free(inputfile);
	

	
	return 0;	
}