#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<pthread.h>
#include<png.h>
#include"fractal.h"
#include"png_writer.h"




int mandelbrot(double cx,double cy)
{
    

    int iter=0;
    double zx=0,zy=0,zy2=0,zx2=0;
    while(zx2+zy2<4.0 && iter<MAX_ITER)
    {
        zy = 2*zx*zy + cy;
        zx = zx2-zy2 + cx;
        zx2 = zx*zx;
        zy2 = zy*zy;
        iter++;
    }
    return iter;
}




static void *render_thread(void *arg)
{
    ThreadData *data=(ThreadData *)arg;
    for(int y =data->start_y;y<data->end_y;y++)
    {
        for(int x=0;x<data->width;x++)
        {
            double cx=data->xmin+ (x/(double)data->width)*(data->xmax-data->xmin);

            double cy=data->ymin+(y/(double)(data->height))*(data->ymax-data->ymin);

            int iter=mandelbrot(cx,cy);
            int offset=(y*data->width+x)*3;

            if(iter==MAX_ITER)
            {
                data->image[offset]=data->image[offset+1]=data->image[offset+2]=0;
            }

            else{
                double t=sqrt((double)iter/MAX_ITER);
                    data->image[offset] = (unsigned char)(40*data->red*t*255);
                    data->image[offset+1] = (unsigned char)(8*(1-t)*data->green*(1-t)*255);
                    data->image[offset+2] = (unsigned char)(83*data->blue*t*255);

            }
        }
       
    }
     return NULL;
}


int generate_png(double xmin,double xmax,double ymin,double ymax,int width,int height,unsigned char **png_data,size_t *png_size,double r,double g,double b)
{
    // printf("wo hen xihuan bing qiling");
    unsigned char *image = malloc(width*height*3);
    if(!image) return 0;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int rows_per_thread=height/NUM_THREADS;

    for(int i=0;i<NUM_THREADS;i++)
    {
        thread_data[i].xmin=xmin;
        thread_data[i].xmax=xmax;
        thread_data[i].ymin = ymin;  
        thread_data[i].ymax = ymax;
        thread_data[i].width = width;
        thread_data[i].height = height;
        thread_data[i].start_y=i*rows_per_thread;
        thread_data[i].end_y=(i==NUM_THREADS-1)?height:(i+1)*rows_per_thread;
        thread_data[i].image=image;
        thread_data[i].red=r;
        thread_data[i].green=g;
        thread_data[i].blue=b;
        pthread_create(&threads[i],NULL,render_thread,&thread_data[i]);

    }

    for(int i=0;i<NUM_THREADS;i++)
    {
        pthread_join(threads[i],NULL);
    }

    png_structp png=png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if(!png){
        free(image);
        return 0;
    }
    png_infop info =png_create_info_struct(png);

    if(!info)
    {
        png_destroy_write_struct(&png,NULL);
        free(image);
        return 0;
    }

    PngBuffer buffer={NULL,0};
    png_set_write_fn(png,&buffer,png_write_callback,NULL);
    png_set_IHDR(png,info,width,height,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png,info);

    png_bytep row_pointers[height];

    for(int y=0;y<height;y++)
          row_pointers[y]=&image[y*width*3];

    png_write_image(png,row_pointers);

    png_write_end(png,NULL);

    *png_data=buffer.data;
    *png_size=buffer.size;

    png_destroy_write_struct(&png,&info);
    
    free(image);
    return 1;
}


