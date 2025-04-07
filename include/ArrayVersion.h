#ifndef ARRAY_V_
#define ARRAY_V

#include "Mandelbrot.h"

void CheckPixelArray  (double* x, double* y, double* old_x, double* x0, double* y0, int* mask, sf::VertexArray& pixels, int counter, int iteration, int xi, int yi);
void DoIterationArray (double* x0, double* y0, sf::VertexArray& pixels, int xi, int yi);
void ArrayVersion     (sf::VertexArray& pixels, double zoom, double offset_y, double offset_x);

#endif
