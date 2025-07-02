#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include "http_server.h"
#include "files.h"


int main(int argc,char *argv[])
{
    
  int port=7000;
  for(int i=0;i<argc;i++)
  {
    if(strcmp(argv[i],"--port")==0){
      port=atoi(argv[i+1]);
    }
  }




  HTTP_server server;
  start_HTTP_server(&server,port);
  add_files(&server);
  printf("Server running on port %d\n",server.port);
  

    while(1)
    {
        int client_socket=accept(server.socket,NULL,NULL);
      if (client_socket < 0) {
            perror("accept failed");
            continue;  
        }
        handle_request(client_socket,&server);
        close(client_socket);
    }
          
   
}