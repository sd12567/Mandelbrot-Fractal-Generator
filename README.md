#  Mandelbrot Fractal Generator

Mandelbrot Fractal Generator is a fractal image renderer built in C. It renders Mandelbrot sets using the escape-time algorithm with multithreaded image generation. The server supports two modes: a full-featured HTTP backend using libmicrohttpd and a minimal HTTP/1.1 parser over raw TCP sockets. Users can view and zoom into fractals via a simple web frontend, with controls for coordinates and color




---

## Key Features

- **Dual back‑end support written in c**
  - **Lightweight HTTP parser (Custom Socket Server)** – manually parses HTTP/1.1 requests made through sockets,ideal for embedded or low‑latency use
  - **Full HTTP server** using [`libmicrohttpd`](https://www.gnu.org/software/libmicrohttpd/) 
- **Interactive Mandelbrot rendering**
  - Center‑preserving zoom
  - Adjustable coordinate ranges and RGB colouring
  - Escape‑time *mandelbrot-fractal* algorithm with smooth colour gradients
- **Multi-threaded image generation**
  - Usage of multiple POSIX threads to parallelize image generation 
- **Simple HTML/JS front‑end**
  - Form inputs for coordinates and colours
  - Live preview of generated fractals
  

---

## Backend Overview

| Backend Type   | Best For           | Difference                      |
|----------------|--------------------|-----------------------------|
| Custom Socket Server | Low‑latency, embedded      | sockets used,manual parsing of requests required  |
| Full HTTP Server    | Web integration         | using the `librmicrohttpd` library   |

---

## Technical Details

- **IEEE 754 double‑precision** maths for zoom capability
- Escape‑time algorithm with smooth colour mapping  
- PNG encoding via **[`libpng`](http://www.libpng.org/pub/png/libpng.html)** (in‑memory write callback)  
- Server built entirely in **C (C17)**, usage of external libraries  [`libmicrohttpd`](https://www.gnu.org/software/libmicrohttpd/) ,  [`libpng`](http://www.libpng.org/pub/png/libpng.html)
- Lightweight frontend built with **vanilla Javascript**,**HTML**, and **CSS**
---


## Core Algorithm

```c
/*
Mandelbrot iteration

For each pixel/complex no (cx,cy) 
we iterate the function:
    z₀ = 0
    zₙ₊₁ = zₙ² + c
*/

int mandelbrot(double cx, double cy)
{
    double zx = 0, zy = 0;
    int n = 0;
    while (zx*zx + zy*zy <= 4.0 && n < MAX_ITER) 

    /*
    Pixel escapes if it the magnitude |√(zx² + zy²)| becomes greater than 2.
    If it does not escape then it has n=MAX_ITER and considered part of the set
    */
     {
        double tmp = zx*zx - zy*zy + cx;
        zy = 2 * zx * zy + cy;
        zx = tmp;
        n++;
    }
    return n;
}

```

---

## Usage
- Build using:
  - `make socket` for the *Custom Socket Server(SocketServer)* version
  - `make mhd` for the *Full HTTP Server(MHDServer)* version
- Run executable
  - `./server_socket` for the *Custom Socket Server*
  - `./server_mhd`for the *Full HTTP Server* 
- To use a custom port
   - `./server_mhd --port port_no_of_your_choice` for *Custom Socket Server*
   - `./server_socket --port port_no_of_your_choice` for *Full HTTP Server*
