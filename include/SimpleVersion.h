#ifndef SIMPLE_V
#define SIMPLE_V

#include "Mandelbrot.h"

inline void CheckPixelSimple (int iteration, int xi, int yi, sf::VertexArray& pixels);
inline void DoIterationSimple(sf::VertexArray& pixels, double x0, double y0, int xi, int yi);
       void SimpleVersion    (sf::VertexArray& pixels, Scale* scale);

#endif
