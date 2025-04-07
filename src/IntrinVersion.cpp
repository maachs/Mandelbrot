#include "IntrinVersion.h"

void CheckPixelIntrin(sf::VertexArray& pixels, int *mask, int *out_mask, int xi, int yi, int iteration)
{
    for (int j = 0; j < BATCH_SIZE; j++)
        {
            if ((*mask & (1 << j)) && !(*out_mask & (1 << j)))
            {
                int index = yi * LENGTH + xi + j;
                pixels[index].color = sf::Color(
                    iteration * MAGIC_CONST_RED % MAX_ITERATION,
                    iteration * MAGIC_CONST_GREEN % MAX_ITERATION,
                    iteration * MAGIC_CONST_BLUE % MAX_ITERATION);

                *out_mask |= (1 << j);
            }
        }
}

void DoIterationIntrin(__m128* x0, __m128* y0, sf::VertexArray& pixels, int xi, int yi)
{
    __m128 x = _mm_setzero_ps();
    __m128 y = _mm_setzero_ps();
    int iteration = 0;
    int out_mask = 0;
    while (iteration < MAX_ITERATION && out_mask != 0x0f)
    {
        __m128 old_x = x;

        __m128 x2 = _mm_mul_ps(x, x);
        __m128 y2 = _mm_mul_ps(x, y);

        x = _mm_add_ps(x2, y2);
        x = _mm_add_ps(x, *x0);

        __m128 xy = _mm_mul_ps(old_x, y);
        y = _mm_mul_ps(xy, two);
        y = _mm_add_ps(y, *y0);

        __m128 R   = _mm_add_ps(_mm_mul_ps(x, x), _mm_mul_ps(y, y));
        __m128 cmp = _mm_cmpge_ps(R, max_r);

        int mask = _mm_movemask_ps(cmp);   //which dots run away in this iteration

        CheckPixelIntrin(pixels, &mask, &out_mask, xi, yi, iteration);
//         for (int j = 0; j < BATCH_SIZE; j++)
//         {
//             if ((mask & (1 << j)) && !(out_mask & (1 << j)))
//             {
//                 int px = xi + j;
//                 int py = yi;
//                 int index = py * LENGTH + px;
//                 pixels[index].color = sf::Color(
//                     iteration * MAGIC_CONST_RED % MAX_ITERATION,
//                     iteration * MAGIC_CONST_GREEN % MAX_ITERATION,
//                     iteration * MAGIC_CONST_BLUE % MAX_ITERATION);
//
//                 out_mask |= (1 << j);
//             }
//         }
        //if(_mm_cmpge_ps(R, max_r)) break;           //if all R >= max_r break
        /*for(int j = 0; j < BATCH_SIZE; j++)
        {
            CheckPixelArray(x, y, old_x, x0, y0, &out_mask, pixels, j, iteration, xi, yi);
        }*/

        iteration++;
    }
}

void IntrinVersion(sf::VertexArray& pixels, double zoom, double offset_y, double offset_x)
{
    for (int yi = BATCH_SIZE; yi < HIGH - BATCH_SIZE; yi+= BATCH_SIZE)
    {
        //__m128 y_ind = _mm_set_ps(y_ind + 3, y_ind + 2, y_ind + 1, y_ind);
        __m128 y0  = _mm_set_ps((yi + 3 - Y0) / (HIGH/ 4.0) / zoom + offset_y,
                                  (yi + 2 - Y0) / (HIGH/ 4.0) / zoom + offset_y,
                                  (yi + 1 - Y0) / (HIGH/ 4.0) / zoom + offset_y,
                                  (yi - Y0)     / (HIGH/ 4.0) / zoom + offset_y);
        //  for(int i = 0; i < BATCH_SIZE; i++)
        // {
        //     y0[i]  = (yi + i - Y0) / (HIGH/ 4.0) / zoom + offset_y;
        // }

        for (int xi = BATCH_SIZE; xi < LENGTH - BATCH_SIZE; xi+= BATCH_SIZE)
        {
            //__m128 x_ind = _mm_set_ps(x_ind + 3, x_ind + 2, x_ind + 1, x_ind);
            __m128 x0  = _mm_set_ps((xi + 3 - X0) / (LENGTH / 4.0) / zoom + offset_x,
                                      (xi + 2 - X0) / (LENGTH / 4.0) / zoom + offset_x,
                                      (xi + 1 - X0) / (LENGTH / 4.0) / zoom + offset_x,
                                      (xi - X0)     / (LENGTH / 4.0) / zoom + offset_x);
            // double x0[BATCH_SIZE] = {};
            // for(int i = 0; i < BATCH_SIZE; i++)
            // {
            //     x0[i] = (xi + i - X0) / (LENGTH / 4.0) / zoom + offset_x;
            // }

            //double x[BATCH_SIZE] = {};
            //double y[BATCH_SIZE] = {};

            DoIterationIntrin(&x0, &y0, pixels, xi, yi);
        }
    }
}
