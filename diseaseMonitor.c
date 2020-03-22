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
		
		if(search(head,  new_entry->recordID) != NULL){
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

	//command prompt
	size_t n;
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
				// stats(diseaseHashTable);
			}
			else
			{
				char * date1 = token; //what 
				printf("%s\n", date1);
				token = strtok(NULL," ");
				if (token == NULL)
					printf("you have to give two dates\n");
				else 
					printf("%s\n",token);
			}

/*2*/	} else if (strncmp(command, "/diseaseFrequency", strlen("/diseaseFrequency")) == 0 || strncmp(command, "df", strlen("df")) == 0) {
			// printf("/diseaseFrequency\n");
			char *token = strtok(command," ");
			if (token == NULL) continue;
    		token = strtok(NULL, "\0");
    		if (token == NULL) continue;
			printf("%s\n",token);
			

/*3*/	} else if (strncmp(command, "/topk-Diseases", strlen("/topk-Diseases")) == 0 || strncmp(command, "/topd", strlen("/topd")) == 0) {
			// printf("/topk-Diseases\n");
			


/*4*/	} else if (strncmp(command, "/topk-Countries", strlen("/topk-Countries")) == 0 || strncmp(command, "topkc", strlen("topkc")) == 0) {
			// printf("/topk-Countries\n");
			
			char *token = strtok(command," ");
    		token = strtok(NULL, " ");
			// printf("%s\n",token);
			

/*5*/	} else if (strncmp(command, "/insertPatientRecord", strlen("/insertPatientRecord")) == 0 || strncmp(command, "ipr", strlen("ipr")) == 0) {
			// printf("insertPatientRecord\n");
			char *cmd = strtok(command," ");
			if (cmd == NULL) continue;
			char *line = strtok(NULL, "\0");
			if (line == NULL) continue;
			entry* new_entry = line_to_entry(line);
			if (new_entry == NULL)
			{
				printf("%s\n", "something was wrong with your entry");
				continue;
			}

			//check whether id already exists
			list_node *retVal = search(head, new_entry->recordID);
			if (retVal != NULL)
			{
				printf("id already exists\n");
				continue;
			}

			list_node *new_node = sortedInsert(&head, new_entry);
			insert_to_hash(diseaseHashTable, diseaseHashNum, new_node->data->diseaseID, new_node, capacity); 
			insert_to_hash(countryHashTable, countryHashNum, new_node->data->country, new_node, capacity);
			print_list(head);



/*6*/	} else if (strncmp(command, "/recordPatientExit", strlen("/recordPatientExit")) == 0 || strncmp(command, "rpe", strlen("rpt")) == 0) {
			// printf("recordPatientExit\n");
			char *recordID = strtok(command," ");
			char *exitDate;
			recordID = strtok(NULL, " ");
			if (recordID == NULL){
				printf("no recordID given\n");
				free(recordID);
				continue;
			}
			else
			{
				exitDate = strtok(NULL, " ");
				if (exitDate == NULL){
					printf("no exitDate given\n");
					free(exitDate);
					continue;
				}
			}
		
			int retVal = recordPatientExit(head, recordID, exitDate);
			if (retVal == -1)
				printf("something was wrong with your command\n");			

/*7*/	} else if (strncmp(command, "/numCurrentPatients", strlen("/numCurrentPatients")) == 0) {
			// printf("numCurrentPatients\n");
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