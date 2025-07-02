#define _POSIX_C_SOURCE 200809L
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include "route.h"
#include "http_server.h"
#include "fractal.h"

void start_HTTP_server(HTTP_server *server,int port)
{
    int server_socket=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = INADDR_ANY,
    .sin_zero = {0}  
};

    int opt=1;
    setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(server_socket,(struct sockaddr*)&addr,sizeof(addr));
    listen(server_socket,10);
    server->routes=malloc(sizeof(HashTable));
    server->port=port;
    server->socket=server_socket;

    create_table(server->routes);
    printf("\nServer started on port %d\n",server->port);
 
}


void send_mandelbrot_image(int client_socket,double xmin,double xmax,double ymin,double ymax,double r,double g,double b)
{
    int width=800,height=600;
    unsigned char *image_data=NULL;
    size_t image_size;
// printf("ubermensch: xmin=%.2f xmax=%.2f ymin=%.2f ymax=%.2f | RGB: r=%.2f g=%.2f b=%.2f\n", 
//        xmin, xmax, ymin, ymax, r, g, b);

    if(!generate_png(xmin,xmax,ymin,ymax,width,height,&image_data,&image_size,r,g,b))
    {
        fprintf(stderr, "Failed to generate PNG\n");
        char *error = "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "Failed to generate fractal image";
        send(client_socket, error, strlen(error), 0);
        return;
    }

    char header[1024];
    snprintf(header,sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: image/png\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n"
                "\r\n",image_size
                    );
    send(client_socket,header,strlen(header),0);

    send(client_socket,image_data,image_size,0);
    free(image_data);
}


void add_route(HTTP_server *server,char *path,char *file)
{
   

    if(!server->routes|| !file)
    {
        return ;
    }
    char file_cpy[256]="";
    strcpy(file_cpy,file);
    
    if(strstr(file_cpy,".html")){
    int len=strlen(file)+1;
    char *full_file_path=malloc(len);
    strcpy(full_file_path,file);
    add_table(server->routes,path,full_file_path);
    }
    else if(strstr(file_cpy,".css")||(strstr(file_cpy,".js")||strstr(file_cpy,".ico"))){
        int len=strlen(file)+1;
        char *full_file_path=malloc(len);
        strcpy(full_file_path,file);
        add_table(server->routes,path,full_file_path);
    }
    else{
        char *full_file_path=""; 
        add_table(server->routes,path,full_file_path);
       }
}

void parse(char *s,double *xmin,double *xmax,double *ymin,double *ymax,double *r,double *g,double *b)
{
    printf("\n%s",s);
    char *pair=strtok(s,"&");
    while(pair)
    {
        printf("\n%s",pair);

        char *eq=strchr(pair,'=');
        *eq='\0';
        eq++;
        char *key=pair;
        char *value=eq;
        
        if(strcmp(key,"xmin")==0) *xmin=atof(value); 
        else if(strcmp(key,"xmax")==0) *xmax=atof(value);
        else  if(strcmp(key,"ymin")==0) *ymin=atof(value);
        else  if(strcmp(key,"ymax")==0) *ymax=atof(value);
        else if(strcmp(key,"red")==0) *r=atof(value);
        else if(strcmp(key,"green")==0) *g=atof(value);
        else if(strcmp(key,"blue")==0) *b=atof(value);



        pair=strtok(NULL,"&");

}printf("Parsed: xmin=%.2f xmax=%.2f ymin=%.2f ymax=%.2f | RGB: r=%.2f g=%.2f b=%.2f\n", 
       *xmin, *xmax, *ymin, *ymax, *r, *g, *b);
}





void handle_request(int client_socket,HTTP_server* server)
{
    static int i=0;
    char request[4096]={0};
    ssize_t n=read(client_socket,request,sizeof(request)-1);
    if(n<=0)
    {
        perror("did not read");
        return;
    }
    request[n]='\0';
    printf("Raw request %d:\n%s\n\n",i++,request);
    fflush(stdout);
    char *firstline=strtok(request,"\r\n");
    strtok(firstline," ");
    char *path=strtok(NULL," ");
    
    char content_type[1024]="";

    if(!path) return;
    if( strstr(path,".js"))
        strcat(content_type,"application/javascript; charset=utf-8");
    
    else if( strstr(path,".css"))   strcat(content_type,"text/css");
    else if( strstr(path,".ico"))   strcat(content_type,"image/x-icon");
    else if(strstr(path,"/mandelbrot")==path)
    {
       
        double xmin=-2.0;
        double xmax=1.0;
        double ymin=-1.5;
        double ymax=1.5;
        double r=0.88;
        double g=0.37;
        double b=0.26;

        char *query=strchr(path,'?');
        printf("#%s#",query);
        if(query){  parse(query+1,&xmin,&xmax,&ymin,&ymax,&r,&g,&b);
            printf("hello");
        }
        send_mandelbrot_image(client_socket,xmin,xmax,ymin,ymax,r,g,b);
        return;
    }
    
    
    else strcat( content_type,"text/html; charset=utf-8");
    Route *route=search_table(server->routes,path);

    if(route)
    {
        char header[8192];
        snprintf(header,sizeof(header),"HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n",content_type);
        send(client_socket,header,strlen(header),0);

        FILE *file=fopen(route->value,"rb");
        if(!file){
            char err[]="HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<h1>404 Not Found</h1>";
            send(client_socket,err,strlen(err),0);
            return;
        }
        char buffer[8192];
        size_t bytes;
        while((bytes=fread(buffer,1,sizeof(buffer),file))>0){
            if(send(client_socket,buffer,bytes,0)<0)
            {
                perror("send failed");
                break;
            }
        }

        fclose(file);
    }
    else {
        char header[]="HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
        char response[8192]={0};
        strcat(response,header);
        strcat(response,"<h1>404 NOT FOUND!</h1>");
        send(client_socket,response,strlen(response),0);
    }
}

void free_memory(HTTP_server *server) {
    if (server->routes) {
        free_table(server->routes);
        free(server->routes);
        free(server);
    }
}