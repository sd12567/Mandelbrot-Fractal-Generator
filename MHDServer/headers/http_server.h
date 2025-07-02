#ifndef HTTP_SERVER
#define HTTP_SERVER

#include<microhttpd.h>
#define PORT 7800
#

int start_server(int );

// enum MHD_Result handle_request(void *cls,struct MHD_Connection *connection,const char*url ,const char *method,const char *version,const char *upload_data,size_t *upload_data_size,void **con_cls);

enum MHD_Result render_file(struct MHD_Connection *connection,const char *path,const char *content_type);
#endif 