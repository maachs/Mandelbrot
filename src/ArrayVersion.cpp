#include <ArrayVersion.h>

void CheckPixelArray(double* x, double* y, double* x0, double* y0, int* mask, sf::VertexArray& pixels, int counter, int iteration, int xi, int yi)
{
    if(!(*mask & (1 << counter)))
    {
        double old_x = x[counter];
        x[counter] = x[counter] * x[counter] - y[counter] * y[counter] + x0[counter];
        y[counter] = 2 * y[counter] * old_x + y0[counter];

        if(x[counter] * x[counter] + y[counter] * y[counter] > MAX_R)
        {
            int index = yi * LENGTH + xi + counter;
            pixels[index].color = sf::Color(
                iteration * MAGIC_CONST_RED % MAX_ITERATION,
                iteration * MAGIC_CONST_GREEN % MAX_ITERATION,
                iteration * MAGIC_CONST_BLUE % MAX_ITERATION);
            *mask |= (1 << counter);
        }
    }
}

void DoIterationArray(double* x0, double* y0, sf::VertexArray& pixels, int xi, int yi)
{
    int iteration = 0;
    int out_mask = 0;

    double x[BATCH_SIZE] = {};
    double y[BATCH_SIZE] = {};

    for(int batch_elem = 0; batch_elem < BATCH_SIZE; batch_elem++)
    {
        x[batch_elem] = x0[batch_elem];
        y[batch_elem] = y0[batch_elem];
    }

    while (iteration < MAX_ITERATION && out_mask != ((1 << BATCH_SIZE) - 1))
    {
        for(int j = 0; j < BATCH_SIZE; j++)
        {
            CheckPixelArray(x, y, x0, y0, &out_mask, pixels, j, iteration, xi, yi);
        }
        iteration++;
    }
}

void ArrayVersion(sf::VertexArray& pixels, Scale* scale)
{
    for (int yi = 0; yi < HIGH; yi++)
    {
        double y0[BATCH_SIZE] = {};
        for(int batch_elem = 0; batch_elem < BATCH_SIZE; batch_elem++)
        {
            y0[batch_elem] = (yi - Y0) / (HIGH / 4.0) / scale->zoom + scale->offset_y;
        }

        for (int xi = 0; xi < LENGTH; xi += BATCH_SIZE)
        {
            double x0[BATCH_SIZE] = {};
            for(int i = 0; i < BATCH_SIZE && (xi + i) < LENGTH; i++)
            {
                x0[i] = (xi + i - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x;
            }
            DoIterationArray(x0, y0, pixels, xi, yi);
        }
    }
}
