#include "SimpleVersion.h"

int main()
{
    struct timeval start, end;
    sf::RenderWindow window(sf::VideoMode(LENGTH, HIGH), "Mandelbrot");
    window.setVerticalSyncEnabled(false);
    sf::VertexArray pixels(sf::Points, LENGTH * HIGH);

    for(int x = 0; x < LENGTH; x++)
    {
        for(int y = 0; y < HIGH; y++)
        {
            int index = x + y * LENGTH;
            pixels[index].position = sf::Vector2f(x, y);
            pixels[index].color = sf::Color::Black;
        }
    }

    gettimeofday(&start, NULL);
    int n_windows = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int xi = 0; xi < LENGTH; xi++)
        {
            double x0 = (xi - X0) / (LENGTH / 4.0);

            for (int yi = 0; yi < HIGH; yi++)
            {
                double y0 = (yi - Y0) / (HIGH / 4.0);

                double x = 0, y = 0;
                int iteration = 0;
                while (x*x + y*y <= MAX_R && iteration < MAX_ITERATION)
                {
                    double old_x = x;
                    x = x*x - y*y + x0;
                    y = 2*old_x*y + y0;
                    iteration++;
                }
                if (iteration < MAX_ITERATION)
                {
                    int index = yi * LENGTH + xi;
                    pixels[index].position = sf::Vector2f(xi, yi);
                    pixels[index].color = sf::Color(
                                               iteration * 5 % MAX_ITERATION,
                                               iteration * 10 % MAX_ITERATION,
                                               iteration * 20 % MAX_ITERATION
                    );
                }
            }
        }

        n_windows++;

        window.clear();
        window.draw(pixels);
        window.display();
    }

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double time_spent = seconds + microseconds * 1e-6;
    double fps = n_windows / time_spent;
    printf("fps %lg\n", fps);
    return 0;
}
