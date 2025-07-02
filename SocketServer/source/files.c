#include "http_server.h"
#include "route.h"


void add_files(HTTP_server* server)
{
  add_route(server,"/","frontend/templates/index.html");
  add_route(server,"/style.css","frontend/static/style.css");
  add_route(server,"/script.js","frontend/static/script.js");
  add_route(server,"/mandelbrot",NULL);
}