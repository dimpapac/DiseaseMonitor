#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"
#include "list.h"
#include "rbt.h"
#include "heap.h"


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

	char *line = NULL;
	size_t len = 0;
	ssize_t lineSize;
	
	int capacity = (bucketSize - sizeof(struct b*) - sizeof(int) )/ sizeof(bucket_entry);
	// printf("capacity %d\n", capacity);
	if (capacity < 1)
	{
		printf("bucket size is too small \n");
		return -1;
	}
	// printf("sizeof bucket entry %lu\n", sizeof(bucket_entry));

	list_node *head = NULL; // head of list 
		
	while ((lineSize = getline(&line, &len, input)) != -1) {
		// printf("lineSize = %lu    len = %lu\n", lineSize, len);
		line[lineSize - 1] = '\0';
		// printf("%s\n", line);
		entry* new_entry = line_to_entry(line);
		if (new_entry == NULL)
		{
			printf("problem\n");
			return -1;
		}
		// print_entry(new_entry);
		
		if(search(head,  new_entry->recordID) != NULL){
			printf("%s already exists\n", new_entry->recordID);
			//free entry
			free(new_entry->recordID);
			free(new_entry->patientFirstName);
			free(new_entry->patientLastName);
			free(new_entry->diseaseID);
			free(new_entry->country);
			free(new_entry);
			free(line);
			line = NULL;
			//free data structures
			free_hash(diseaseHashTable, diseaseHashNum);
			free_hash(countryHashTable, countryHashNum);
			free_list(head);
			//close file
			fclose(input);
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
		//insert patient to a sorted list
		list_node *new_node = sortedInsert(&head, new_entry);
		//insert patient to hash tables
		insert_to_hash(diseaseHashTable, diseaseHashNum, new_node->data->diseaseID, new_node, capacity); 
		insert_to_hash(countryHashTable, countryHashNum, new_node->data->country, new_node, capacity);
		
		free(line);
		line = NULL;
	}

	// print_list(head);

	//command prompt
	size_t n;
	char *command = NULL;
	do {
		if (command != NULL)
			free(command);
		command = NULL;
		lineSize = getline(&command, &n, stdin); //get line from stdin
		command[lineSize - 1] = '\0';
		// printf("COMMAND: %s\n", command); 
/*1*/	if (strncmp(command, "/globalDiseaseStats", strlen("/globalDiseaseStats")) == 0 || strncmp(command, "gds", strlen("gds")) == 0) {
			// printf("/globalDiseaseStats\n");
			char *token = strtok(command," ");
			if (token == NULL) continue;
    		token = strtok(NULL, " ");
			if (token == NULL) //no dates given
			{
				stats(diseaseHashTable, diseaseHashNum);
			}
			else
			{
				char * date1 = token; 
				// printf("%s\n", date1);
				char *date2 = strtok(NULL," ");
				if (date2 == NULL){
					printf("you have to give two dates\n");
					continue;
				}
				else {
					// printf("%s\n",date2);
					stats2dates(diseaseHashTable, diseaseHashNum, date1, date2);
				}
			}

/*2*/	} else if (strncmp(command, "/diseaseFrequency", strlen("/diseaseFrequency")) == 0 || strncmp(command, "df", strlen("df")) == 0) {
			// printf("/diseaseFrequency\n");
			char *token = strtok(command," ");
			if (token == NULL) continue;
    		// printf("%s\n", token);
    		char *virusName = strtok(NULL, " ");
    		if (virusName == NULL) {
    			printf("you have to give a virusName\n");
    			continue;
			}// printf("%s\n", virusName);

			char *date1 = strtok(NULL," ");
			if (date1 == NULL){
				printf("you have to give two dates\n");
				continue;
			}

			char *date2 = strtok(NULL," ");
			if (date2 == NULL){
				printf("you have to give two dates\n");
				continue;
			}

			char *country = strtok(NULL," ");
			if (country == NULL)
			{
				// printf("no country \n");
				frequency(diseaseHashTable, diseaseHashNum, date1, date2, virusName);
				
			}
			else
			{
				// printf("country: %s\n", country);
				frequencyWithCountry(countryHashTable, countryHashNum, date1, date2, virusName, country);

			}

			

/*3*/	} else if (strncmp(command, "/topk-Diseases", strlen("/topk-Diseases")) == 0 || strncmp(command, "topkd", strlen("topkd")) == 0) {
			// printf("/topk-Diseases\n");
			char *token = strtok(command," "); //topk
			if (token == NULL) continue;
    		char *ks = strtok(NULL, " ");
    		if (ks == NULL){
    			printf("you have to give a number\n");
    			continue;
    		}
    		int k = atoi(ks);
    		if (k == 0)
    		{
    			printf("k must be a number\n");
    			continue;
    		}
    		char *country = strtok(NULL, " ");
    		if (country == NULL){
    			printf("you have to give a country\n");
    			continue;
    		}
    		char *date1 = strtok(NULL, " ");
			if (date1 == NULL) //no dates given
			{
				// printf("no dates\n");
				date nodate; //NULL 
				topk(countryHashTable, countryHashNum, k, country, 0, nodate, nodate);
			}
			else
			{
				// printf("%s\n", date1);
				char *date2 = strtok(NULL," ");
				if (date2 == NULL){
					printf("you have to give two dates\n");
					continue;
				}
				else {
					date idate1, idate2;
					if (charToDate(date1, &idate1) != 0 || charToDate(date2, &idate2) != 0 || earlier(&idate2, &idate1) == 1)
					{
						printf("wrong dates\n");
						continue;
					}
					
					topk(countryHashTable, countryHashNum, k, country, 2, idate1, idate2);
				}
			}


/*4*/	} else if (strncmp(command, "/topk-Countries", strlen("/topk-Countries")) == 0 || strncmp(command, "topkc", strlen("topkc")) == 0) {
			// printf("/topk-Countries\n");
			char *token = strtok(command," "); //topk
			if (token == NULL) continue;
    		char *ks = strtok(NULL, " ");
    		if (ks == NULL){
    			printf("you have to give a number\n");
    			continue;
    		}
    		int k = atoi(ks);
    		if (k == 0)
    		{
    			printf("k must be a number\n");
    			continue;
    		}
    		char *disease = strtok(NULL, " ");
    		if (disease == NULL){
    			printf("you have to give a disease\n");
    			continue;
    		}
    		char *date1 = strtok(NULL, " ");
			// if (token != NULL) printf("%s\n",token);
			if (date1 == NULL) //no dates given
			{
				// printf("no dates\n");
				date nodate; //NULL
				topk(diseaseHashTable, diseaseHashNum, k, disease, 1, nodate, nodate);
			}
			else
			{
				// printf("%s\n", date1);
				char *date2 = strtok(NULL," ");
				if (date2 == NULL){
					printf("you have to give two dates\n");
					continue;
				}
				else {
					date idate1, idate2;
					if (charToDate(date1, &idate1) != 0 || charToDate(date2, &idate2) != 0 || earlier(&idate2, &idate1) == 1)
					{
						printf("wrong dates\n");
						continue;
					}

					topk(diseaseHashTable, diseaseHashNum, k, disease, 3, idate1, idate2);

				}
			}

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
				// free(line);
				line = NULL;
				continue;
			}

			//check whether id already exists
			list_node *retVal = search(head, new_entry->recordID);
			if (retVal != NULL)
			{
				printf("id already exists\n");
				free(new_entry->recordID);
				free(new_entry->patientFirstName);
				free(new_entry->patientLastName);
				free(new_entry->diseaseID);
				free(new_entry->country);
				free(new_entry);
				line = NULL;
				continue;
			}

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
				// free(line);
				line = NULL;
				continue;
			}

			list_node *new_node = sortedInsert(&head, new_entry);
			insert_to_hash(diseaseHashTable, diseaseHashNum, new_node->data->diseaseID, new_node, capacity); 
			insert_to_hash(countryHashTable, countryHashNum, new_node->data->country, new_node, capacity);
			// print_list(head);
			printf("Record added\n");



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
			if (retVal == 0)
				printf("Record updated\n");
			else
				printf("something was wrong with your command\n");			


/*7*/	} else if (strncmp(command, "/numCurrentPatients", strlen("/numCurrentPatients")) == 0 || strncmp(command, "ncp", strlen("ncp")) == 0) {
			// printf("numCurrentPatients\n");
			char *token = strtok(command," "); ///numCur...
			if (token == NULL) continue;
    		char * disease = strtok(NULL, " ");
			if (disease == NULL)
			{
				currentPatients(diseaseHashTable, diseaseHashNum);
			}
			else
			{
				currentPatientsWithDisease(diseaseHashTable, diseaseHashNum, disease);
			}

			
			
		} else if (strcmp(command, "/exit\0") != 0){
			printf("Wrong command try again\n");
		}

	} while(strcmp(command, "/exit\0") != 0);
	printf("exiting\n");
	if (command != NULL)
		free(command);

	// print_hash(diseaseHashTable, diseaseHashNum);
	// print_hash(countryHashTable, countryHashNum);

	//free data structures
	free_hash(diseaseHashTable, diseaseHashNum);
	free_hash(countryHashTable, countryHashNum);

	free_list(head);
	free(line);
	line = NULL;
	//close file
	fclose(input); 
	free(inputfile);
	

	
	return 0;	
}