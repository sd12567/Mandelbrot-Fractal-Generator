#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "route.h"

typedef struct{
    int socket;
    int port;
    HashTable *routes;
}HTTP_server;

void start_HTTP_server(HTTP_server *,int);
void add_route(HTTP_server *,char *,char *);
void handle_request(int ,HTTP_server* );
void free_memory(HTTP_server* );


#endif