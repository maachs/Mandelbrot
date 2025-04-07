#ifndef INTRIN_V_
#define INTRIN_V_

#include "Mandelbrot.h"

const __m128 two = _mm_set_ps1(2);
const __m128 max_r = _mm_set_ps1(MAX_R);

void CheckPixelIntrin(sf::VertexArray& pixels, int *mask, int *out_mask, int xi, int yi, int iteration);
void DoIterationIntrin(__m128* x0, __m128* y0, sf::VertexArray& pixels, int xi, int yi);
void IntrinVersion(sf::VertexArray& pixels, double zoom, double offset_y, double offset_x);

#endif
