#ifndef ARRAY_V_
#define ARRAY_V

#include "Mandelbrot.h"

inline void CheckPixelArray  (double* x, double* y, double* x0, double* y0, int* mask, sf::VertexArray& pixels, int counter, int iteration, int xi, int yi);
inline void DoIterationArray (double* x0, double* y0, sf::VertexArray& pixels, int xi, int yi);
       void ArrayVersion     (sf::VertexArray& pixels, Scale* scale);

#endif
