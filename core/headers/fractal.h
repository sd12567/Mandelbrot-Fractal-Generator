#ifndef FRACTAL_H
#define FRACTAL_H

#include<stdlib.h>
#include<math.h>
#include<png.h>
#include<string.h>
#include<pthread.h>

#define MAX_ITER 1000
#define NUM_THREADS 16

typedef struct {
    double xmin,xmax,ymin,ymax;
    int width;
    int height;
    int start_y,end_y;
    unsigned char* image;
    double red,green,blue;
} ThreadData;



int generate_png(double xmin,double xmax,double ymin,double ymax,int width,int height,unsigned char **png_data,size_t *png_size,double r,double g,double b);



int mandelbrot(double cx,double cy);
void png_write_callback(png_structp png_ptr,png_bytep data,png_size_t length);



#endif 
