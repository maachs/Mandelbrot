#include "IntrinVersion.h"

void CheckPixelIntrin(sf::VertexArray& pixels, int points, int *out_mask, int xi, int yi, int iteration)
{
    for (int j = 0; j < BATCH_SIZE; j++)
    {
        if (points & (1 << j))
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

void DoIterationIntrin(__m128 x0, __m128 y0, sf::VertexArray& pixels, int xi, int yi)
{
    __m128 x = x0;
    __m128 y = y0;
    __m128i iterations = _mm_setzero_si128();

    int out_mask = 0;
    while (_mm_movemask_ps(_mm_castsi128_ps(_mm_cmplt_epi32(iterations, ITERATION_MAX))) && (out_mask != ((1 << BATCH_SIZE) - 1)))
    {
        __m128 old_x = x;

        __m128 x_sq = _mm_mul_ps(x, x);
        __m128 y_sq = _mm_mul_ps(y, y);

        x = _mm_add_ps(_mm_sub_ps(x_sq, y_sq), x0);

        __m128 xy = _mm_mul_ps(old_x, y);
        y = _mm_add_ps(_mm_mul_ps(xy, TWO), y0);

        __m128 r_sq = _mm_add_ps(_mm_mul_ps(x, x), _mm_mul_ps(y, y));

        __m128 cmp = _mm_cmpge_ps(r_sq, R_MAX);

        int new_mask = _mm_movemask_ps(cmp);                    //which dots run away in this iteration
        int new_points = new_mask & (~out_mask);                //cmp with dots which run away before

        if (new_points)
        {
            CheckPixelIntrin(pixels, new_points, &out_mask, xi, yi, _mm_extract_epi16(iterations, 0));
        }
        iterations = _mm_add_epi32(iterations, INCREMENT);
    }
}

void IntrinVersion(sf::VertexArray& pixels, Scale* scale)
{
    for (int yi = BATCH_SIZE; yi < HIGH; yi++)
    {
        __m128 y0 = _mm_set_ps1((yi - Y0) / (HIGH / 4.0) / scale->zoom + scale->offset_y);

        for (int xi = BATCH_SIZE; xi < LENGTH - BATCH_SIZE; xi += BATCH_SIZE)
        {
            __m128 x0 = _mm_set_ps(((xi + 3 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x),
                                   ((xi + 2 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x),
                                   ((xi + 1 - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x),
                                   ((xi - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x));

            DoIterationIntrin(x0, y0, pixels, xi, yi);
        }
    }
}
