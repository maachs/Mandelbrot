#include "ArrayVersion.h"

void CheckPixelArray(double* x, double* y, double* old_x, double* x0, double* y0, int* mask, sf::VertexArray& pixels, int counter, int iteration, int xi, int yi)
{
    if(!(*mask & (1 << counter)))
    {
        old_x[counter] = x[counter];
        x[counter] = x[counter] * x[counter] - y[counter] * y[counter] + x0[counter];
        y[counter] = 2*y[counter] * old_x[counter] + y0[counter];

        if(x[counter] * x[counter] + y[counter] * y[counter] > MAX_R)
        {
            int px = xi + counter;
            int py = yi;
            for(int elem = 0; elem < BATCH_SIZE; elem++)
            {
                int index = py * LENGTH + px;
                pixels[index].color = sf::Color(
                            iteration * MAGIC_CONST_RED % MAX_ITERATION,
                            iteration * MAGIC_CONST_GREEN % MAX_ITERATION,
                            iteration * MAGIC_CONST_BLUE % MAX_ITERATION);
                *mask |= (1 << counter);
            }
        }
    }
}

void DoIterationArray(double* x, double* y, double* x0, double* y0, sf::VertexArray& pixels, int xi, int yi)
{
    int iteration = 0;
    int out_mask = 0;
    while (iteration < MAX_ITERATION && out_mask != 0x0f)
    {
        double old_x[BATCH_SIZE] = {};
        for(int j = 0; j < BATCH_SIZE; j++)
        {
            CheckPixelArray(x, y, old_x, x0, y0, &out_mask, pixels, j, iteration, xi, yi);
        }

        iteration++;
    }
}

void ArrayVersion(sf::VertexArray& pixels, double zoom, double offset_y, double offset_x)
{
   for (int yi = BATCH_SIZE; yi < HIGH - BATCH_SIZE; yi+= BATCH_SIZE)
    {
        double y0[BATCH_SIZE] = {};
        for(int i = 0; i < BATCH_SIZE; i++)
        {
            y0[i]  = (yi + i - Y0) / (HIGH/ 4.0) / zoom + offset_y;
        }

        for (int xi = BATCH_SIZE; xi < LENGTH - BATCH_SIZE; xi+= BATCH_SIZE)
        {
            double x0[BATCH_SIZE] = {};
            for(int i = 0; i < BATCH_SIZE; i++)
            {
                x0[i] = (xi + i - X0) / (LENGTH / 4.0) / zoom + offset_x;
            }

            double x[BATCH_SIZE] = {};
            double y[BATCH_SIZE] = {};

            DoIterationArray(x, y, x0, y0, pixels, xi, yi);
        }
    }
}
