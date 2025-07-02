#include"http_server.h"
#include<string.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int port=9090;
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i],"--port")==0)
        {
            port=atoi(argv[i+1]);
        }
    }


    start_server(port);
    return 0;
}