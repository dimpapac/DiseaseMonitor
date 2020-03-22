#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "structs.h"
#include "list.h"

void print_entry(entry *my_entry);
entry* line_to_entry(char *line);

int hash1(char *str, int factor);
int hash2(char *str, int factor);

void set_date(int mday, int mmonth, int myear, date *my_date);
int earlier(date *date1, date *date2);


rb_node **diseaseIDexists(bucket *my_bucket, char *my_diseaseID);

bucket * initialize_bucket(int cap);
void insert_entry_to_bucket(bucket *last_bucket, char *diseaseID, list_node *new_node, int capacity);
void insert_to_hash(bucket **diseaseHashTable, int diseaseHashNum, char *string, list_node *new_node, int capacity);

void print_hash(bucket **HashTable, int HashNum);


void free_bucket(bucket *buc);
void free_hash(bucket **diseaseHashTable, int diseaseHashNum);

////////////CLI functions/////////////////////////

int recordPatientExit(list_node* head, char* recordID, char* exitDate);



#endif