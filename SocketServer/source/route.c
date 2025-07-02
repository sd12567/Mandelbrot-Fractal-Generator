#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "route.h"
#include<string.h>


void create_table(HashTable *t)
{
   t->size=128;
   t->table=calloc(t->size,sizeof(Route*));
   
}


int hash(char *key)
{
    unsigned int value=0;
    for(int i=0;key[i]!='\0';i++)
    {
        value=value*69+key[i];
    }
    return value%128;
}



// void add_table(HashTable *t,char *key,char *value)
// {
//     int index =hash(key);
//     printf("index: %d",index);

//     Route* route=malloc(sizeof(Route));

//     route->key=strdup(key);
//     route->value=strdup(value);
  
//     if (!key || !value) {
//         fprintf(stderr, "Error: NULL key or value passed\n");
//         return;
//     }
//     route->next=NULL;

//     if(!t) {printf("hi");return;}

//     if(t->table[index]==NULL)
//         t->table[index]=route;
    

//     else 
//     {
//         Route* current=t->table[index];
//         while(current->next)
//         current=current->next;

//         current->next=route;
//     }

    

// }
void add_table(HashTable *t, char *key, char *value) {
 
    // if (!t || !t->table) {
    //     fprintf(stderr, "Error: HashTable not initialized\n");
    //     exit(EXIT_FAILURE);
    // }
    // if (!key || !value) {
    //     fprintf(stderr, "Error: NULL key or value\n");
    //     return;
    // }


    int index = hash(key);
    if (index < 0 || index >= t->size) {
        fprintf(stderr, "Error: Invalid hash index %d\n", index);
        return;
    }


    Route* route = malloc(sizeof(Route));
    

    route->key = strdup(key);
    route->value = value;
    if (!route->key || !route->value) {
        free(route->key);
        free(route->value);
        free(route);
        perror("Failed to duplicate strings");
        exit(EXIT_FAILURE);
    }
    route->next = NULL;

  
    if (!t->table[index]) {
        t->table[index] = route;  
    } else {
        
        Route* current = t->table[index];
        while (current->next) {
            current = current->next;
        }
        current->next = route;
    }
}


Route *search_table(HashTable *t,char *key)
{
    int index=hash(key);

    Route *current=t->table[index];
    while(current)
    {
        if(strcmp(key,current->key)==0) return current;

        current=current->next;
    }
    return NULL;
}


void free_table(HashTable *t) {
    if (!t) return;

    if (t->table) {
        for (int i = 0; i < t->size; i++) {
            Route *current = t->table[i];
            while (current) {
                Route *temp = current;
                current = current->next;
                free(temp->key);
                free(temp->value);
                free(temp);
            }
        }
        free(t->table);  
        t->table = NULL;
    }
}
