#include "SimpleVersion.h"

void CheckPixelSimple(int iteration, int xi, int yi, sf::VertexArray& pixels)
{
    if (iteration < MAX_ITERATION)
    {
        int index = yi * LENGTH + xi;
        pixels[index].position = sf::Vector2f(xi, yi);
        pixels[index].color = sf::Color(iteration * MAGIC_CONST_RED % MAX_ITERATION,
                                        iteration * MAGIC_CONST_GREEN % MAX_ITERATION,
                                        iteration * MAGIC_CONST_BLUE % MAX_ITERATION);
    }
}

void DoIterationSimple(sf::VertexArray& pixels, double x0, double y0, int xi, int yi)
{
    double x = x0, y = y0;
    int iteration = 0;
    double old_x = 0;
    while ((x*x + y*y <= MAX_R) && (iteration < MAX_ITERATION))
    {
        old_x = x;
        x = x*x - y*y + x0;
        y = 2*old_x*y + y0;
        iteration++;
    }
    CheckPixelSimple(iteration, xi, yi, pixels);
}

void SimpleVersion(sf::VertexArray& pixels, Scale* scale)
{
   for (int xi = 0; xi < LENGTH; xi++)
    {
        double x0 = (xi - X0) / (LENGTH / 4.0) / scale->zoom + scale->offset_x;

        for (int yi = 0; yi < HIGH; yi++)
        {
            double y0 = (yi - Y0) / (HIGH / 4.0) / scale->zoom + scale->offset_y;

            int iteration = 0;

            DoIterationSimple(pixels, x0, y0, xi, yi);
        }
    }
}
