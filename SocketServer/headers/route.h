#ifndef ROUTE_H
#define ROUTE_H

#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>


typedef struct Route{
    char *key;
    char *value;
    struct Route* next;
}Route;


typedef struct{
    Route** table;
    int size;
    
}HashTable;

void create_table(HashTable *);
int hash(char *);
void add_table(HashTable *,char *,char *);
Route *search_table(HashTable *,char *);
void free_table(HashTable *);



#endif 
