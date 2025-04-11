#ifndef INTRIN_V_
#define INTRIN_V_

#include "Mandelbrot.h"

const __m128 TWO = _mm_set_ps1(2.0f);
const __m128 R_MAX = _mm_set_ps1(MAX_R);
const __m128i ITERATION_MAX = _mm_set1_epi32(MAX_ITERATION);
const __m128i INCREMENT = _mm_set1_epi32(1);

inline void CheckPixelIntrin (sf::VertexArray& pixels, int points, int *out_mask, int xi, int yi, int iteration);
inline void DoIterationIntrin(__m128 x0, __m128 y0, sf::VertexArray& pixels, int xi, int yi);
       void IntrinVersion    (sf::VertexArray& pixels, Scale* scale);

#endif
