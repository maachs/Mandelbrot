#include "IntrinVersion.h"

void CheckPixelIntrin(sf::VertexArray& pixels, int mask, int *out_mask, int xi, int yi, int iteration)
{
    for (int j = 0; j < BATCH_SIZE; j++)
        {
            if (mask & (1 << j))
            {
                int index = yi * LENGTH + xi + j;
                pixels[index].position = sf::Vector2f(xi + j, yi);
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
    __m128 x = *x0;
    __m128 y = *y0;
    int iteration = 0;
    int out_mask = 0;
    while (iteration < MAX_ITERATION && out_mask != 0x0f)
    {
        __m128 old_x = x;

        __m128 x2 = _mm_mul_ps(x, x);
        __m128 y2 = _mm_mul_ps(y, y);

        x = _mm_add_ps(_mm_add_ps(x2, y2), *x0);

        __m128 xy = _mm_mul_ps(old_x, y);
        y = _mm_add_ps(_mm_mul_ps(xy, two), *y0);

        __m128 R   = _mm_add_ps(_mm_mul_ps(x, x), _mm_mul_ps(y, y));
        __m128 cmp = _mm_cmpge_ps(R, max_r);

        int mask = _mm_movemask_ps(cmp);                    //which dots run away in this iteration
        int new_points = mask & (~out_mask);                //cmp with dots which run away before

        if (new_points) {
            CheckPixelIntrin(pixels, new_points, &out_mask, xi, yi, iteration);
        }
        //CheckPixelIntrin(pixels, &mask, &out_mask, xi, yi, iteration);
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

void IntrinVersion(sf::VertexArray& pixels, Scale* scale)
{
    __m128 scale_y = _mm_set_ps1(1 / (HIGH / 4.0) / scale->zoom);
    __m128 scale_x = _mm_set_ps1(1 / (LENGTH / 4.0) / scale->zoom);
    __m128 offset_y = _mm_set_ps1(scale->offset_y - Y0/(HIGH / 4.0) / scale->zoom);
    __m128 offset_x = _mm_set_ps1(scale->offset_x - X0/(LENGTH / 4.0) / scale->zoom);

    for (int yi = BATCH_SIZE; yi < HIGH - BATCH_SIZE; yi += BATCH_SIZE)
    {
        __m128 y_idx = _mm_set_ps(yi+3, yi+2, yi+1, yi);
        __m128 y0    = _mm_add_ps(_mm_mul_ps(y_idx, scale_y), offset_y);

        for (int xi = BATCH_SIZE; xi < LENGTH - BATCH_SIZE; xi += BATCH_SIZE)
        {
            __m128 x_idx = _mm_set_ps(xi+3, xi+2, xi+1, xi);
            __m128 x0    = _mm_add_ps(_mm_mul_ps(x_idx, scale_x), offset_x);

            DoIterationIntrin(&x0, &y0, pixels, xi, yi);
        }
    }
//     for (int yi = BATCH_SIZE; yi < HIGH - BATCH_SIZE; yi+= BATCH_SIZE)
//     {
//         //__m128 y_ind = _mm_set_ps(y_ind + 3, y_ind + 2, y_ind + 1, y_ind);
//         __m128 y0  = _mm_set_ps((yi + 3 - Y0) / (HIGH/ 4.0) / scale->zoom + scale->offset_y,
//                                   (yi + 2 - Y0) / (HIGH/ 4.0) / scale->zoom + scale->offset_y,
//                                   (yi + 1 - Y0) / (HIGH/ 4.0) / scale->zoom + scale->offset_y,
//                                   (yi - Y0)     / (HIGH/ 4.0) / scale->zoom + scale->offset_y);
//         //  for(int i = 0; i < BATCH_SIZE; i++)
//         // {
//         //     y0[i]  = (yi + i - Y0) / (HIGH/ 4.0) / zoom + offset_y;
//         // }
//
//         for (int xi = BATCH_SIZE; xi < LENGTH - BATCH_SIZE; xi+= BATCH_SIZE)
//         {
//             //__m128 x_ind = _mm_set_ps(x_ind + 3, x_ind + 2, x_ind + 1, x_ind);
//             __m128 x0  = _mm_set_ps((xi + 3 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x,
//                                       (xi + 2 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x,
//                                       (xi + 1 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x,
//                                       (xi - X0)     / (LENGTH / 4.0) / scale->zoom + scale->offset_x);
            // double x0[BATCH_SIZE] = {};
            // for(int i = 0; i < BATCH_SIZE; i++)
            // {
            //     x0[i] = (xi + i - X0) / (LENGTH / 4.0) / zoom + offset_x;
            // }

            //double x[BATCH_SIZE] = {};
            //double y[BATCH_SIZE] = {};

            //DoIterationIntrin(&x0, &y0, pixels, xi, yi);
        //}
   // }
}
