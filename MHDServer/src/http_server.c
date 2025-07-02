#include "http_server.h"
#include "fractal.h"
// #include "utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<microhttpd.h>


enum MHD_Result render_file(struct MHD_Connection *connection,const char *path,const char *content_type)
{
    FILE *file_ptr=fopen(path,"rb");
      if(!file_ptr) return MHD_NO;

        fseek(file_ptr,0,SEEK_END);
        long file_size=ftell(file_ptr);
        fseek(file_ptr,0,SEEK_SET);

        char *file_data=malloc(file_size+1);
        size_t bytes=fread(file_data,1,file_size,file_ptr);
        if(bytes!=(size_t)file_size)
        {
            free(file_data);
            fclose(file_ptr);
            return MHD_NO;
        }
        fclose(file_ptr);


        struct MHD_Response *response=MHD_create_response_from_buffer(file_size,file_data,MHD_RESPMEM_MUST_FREE);

        MHD_add_response_header(response,"Content-Type",content_type);
        int ret=MHD_queue_response(connection,MHD_HTTP_OK,response);
        MHD_destroy_response(response);
        return ret;

}

enum MHD_Result handle_request(void *cls,struct MHD_Connection *connection,const char *url,const char *method,const char *version,const char *upload_data,size_t *upload_data_size,void **con_cls)

{
    
    (void)cls;
    (void)version;
    (void)upload_data;
    (void)upload_data_size;
    (void)con_cls;

    if(strcmp(method,"GET")!=0) return MHD_NO;


    if(strcmp(url,"/")==0)
    {
      return render_file(connection,"frontend/templates/index.html","text/html");
    }
    else if(strcmp(url,"/script.js")==0){
       return render_file(connection,"frontend/static/script.js","application/javascript");
    }
    else if(strcmp(url,"/style.css")==0){
         return render_file(connection,"frontend/static/style.css","text/css");
    }
    else if(strncmp(url,"/mandelbrot",11)==0){
        const char *xmin_s =MHD_lookup_connection_value(connection,MHD_GET_ARGUMENT_KIND,"xmin");
        const char *xmax_s = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "xmax");
        const char *ymin_s = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "ymin");
        const char *ymax_s = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "ymax");
        const char*red_s=MHD_lookup_connection_value(connection,MHD_GET_ARGUMENT_KIND,"red");
        const char *green_s=MHD_lookup_connection_value(connection,MHD_GET_ARGUMENT_KIND,"green");
        const char *blue_s=MHD_lookup_connection_value(connection,MHD_GET_ARGUMENT_KIND,"blue");
        printf("Request URL: %s\n", url);
        printf("xmin=%s  xmax=%s  ymin=%s  ymax=%s  red=%s  green=%s  blue=%s\n",
       xmin_s  ? xmin_s  : "(null)",
       xmax_s  ? xmax_s  : "(null)",
       ymin_s  ? ymin_s  : "(null)",
       ymax_s  ? ymax_s  : "(null)",
       red_s   ? red_s   : "(null)",
       green_s ? green_s : "(null)",
       blue_s  ? blue_s  : "(null)");
        double xmin = xmin_s ? atof(xmin_s) : -2.0;
        double xmax = xmax_s ? atof(xmax_s) : 1.0;
        double ymin = ymin_s ? atof(ymin_s) : -1.5;
        double ymax = ymax_s ? atof(ymax_s) : 1.5;
        double red=red_s ? atof(red_s):0.69;
        double green=green_s ? atof(green_s):0.69;
        double blue=blue_s?atof(blue_s):0.69;
       

        unsigned char *png_data=NULL;
        size_t png_size;

        if(!generate_png(xmin,xmax,ymin,ymax,800,600,&png_data,&png_size,red,green,blue))
        {
            if(png_data) free(png_data);
            return MHD_NO;
        }

        struct MHD_Response *response =MHD_create_response_from_buffer(png_size,png_data,MHD_RESPMEM_MUST_FREE);

        MHD_add_response_header(response,"Content-Type","image/png");
        enum MHD_Result ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
        MHD_destroy_response(response);
        

        return ret;


    }
    return MHD_NO;
}


int start_server(int port)
{
    struct MHD_Daemon *daemon=MHD_start_daemon(MHD_USE_AUTO|MHD_USE_INTERNAL_POLLING_THREAD,port,NULL,NULL,&handle_request,NULL,MHD_OPTION_END);

    if(!daemon){
        fprintf(stderr,"Failed to start server\n");
        return 1;
    }

    printf("Server running on http://localhost:%d\n",port);
    printf("http://localhost:%d/?xmin=-2&xmax=1&ymin=-1.5&ymax=1.5&color=A\n",port);

    getchar();
    MHD_stop_daemon(daemon);
    return 0;
}