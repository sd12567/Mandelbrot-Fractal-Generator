#include<string.h>
#include<png.h>
#include"png_writer.h"

void png_write_callback(png_structp png_ptr,png_bytep data,png_size_t length)
{
    PngBuffer *buffer=(PngBuffer*)png_get_io_ptr(png_ptr);
    buffer->data=realloc(buffer->data,buffer->size+length);
    memcpy(buffer->data+buffer->size,data,length);
    buffer->size+=length;
}
