#ifndef SIMPLE_V
#define SIMPLE_V

#include "Mandelbrot.h"

void CheckPixelSimple (int iteration, int xi, int yi, sf::VertexArray& pixels);
void DoIterationSimple(sf::VertexArray& pixels, double x0, double y0, int xi, int yi);
void SimpleVersion    (sf::VertexArray& pixels, double zoom, double offset_y, double offset_x);

#endif
