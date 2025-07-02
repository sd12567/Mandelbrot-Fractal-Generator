#  Mandelbrot Fractal Generator

An interactive Mandelbrot Fractal Generator in C with Dual Protocol Support




---

## Key Features

- **Dual back‑end support written in c**
  - **Raw TCP socket server** – minimalist binary protocol, ideal for embedded or low‑latency use
  - **HTTP server with** [`libmicrohttpd`](https://www.gnu.org/software/libmicrohttpd/) 
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

| Backend Type   | Transport Protocol | Best For                     |
|----------------|--------------------|-----------------------------|
| Raw TCP Socket | Custom binary      | Low‑latency, embedded usage |
| HTTP Server    | HTTP / 1.1         | Web integration    |

---

## Technical Details

- **IEEE 754 double‑precision** maths for deep zoom accuracy  
- Escape‑time algorithm with smooth colour mapping  
- PNG encoding via **[`libpng`](http://www.libpng.org/pub/png/libpng.html)** (in‑memory write callback)  
- 100 % C (C17), usage of external libraries  [`libmicrohttpd`](https://www.gnu.org/software/libmicrohttpd/) ,  [`libpng`](http://www.libpng.org/pub/png/libpng.html)
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
