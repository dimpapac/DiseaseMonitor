#ifndef STRUCTS
#define STRUCTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "list.h"
// #include "functions.h"

// typedef struct li_node list_node;



typedef struct {
	int day;
	int month;
	int year;
} date;

typedef struct {
	char *recordID;
	char *patientFirstName; 
	char *patientLastName;
	char *diseaseID;
	char *country;
	date entryDate;
	date exitDate;
} entry;

typedef struct li_node {
	entry *data;
	struct li_node *next;
} list_node;

typedef struct rbt_node{
	//data of node	
	list_node *listPtr; //pointer to patients-list
	date data_date; //date of node 
	int counter; //number of duplicates 	

	int color; //1 if red 
	struct rbt_node *left, *right, *parent;
} rb_node;

typedef struct {
	char *nameOfdisease;
	rb_node *root;
} bucket_entry;

typedef struct b{
	int currentNumberOfEntries;
	bucket_entry *entries; //array of entries

	struct b *next;
} bucket;


// capacity = (bucketSize - sizeof(struct b*) - sizeof(int) )/ sizeof(bucket_entry)




#endif