#include "ArrayVersion.h"

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

    while (window.isOpen())
    {
        gettimeofday(&start, NULL);
        int n_windows = 0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int yi = 4; yi < HIGH - 4; yi+= 4)
        {
            double y0[4] = {};
            for(int i = 0; i < 4; i++)
            {
                y0[i]  = (yi + i - Y0) / (HIGH/ 4.0);
            }

            for (int xi = 4; xi < LENGTH - 4; xi+= 4)
            {
                double x0[4] = {};
                for(int i = 0; i < 4; i++)
                {

                    x0[i] = (xi + i - X0) / (LENGTH/ 4.0);
                }

                double x[4] = {};
                double y[4] = {};

                int iteration = 0;
                int out_mask = 0;
                while (iteration < MAX_ITERATION && out_mask != 0x0f)
                {
                    double old_x[4] = {};
                    for(int j = 0; j < 4; j++)
                    {
                        if(!(out_mask & (1 << j)))
                        {
                            old_x[j] = x[j];
                            x[j] = x[j] * x[j] - y[j] * y[j] + x0[j];
                            y[j] = 2*y[j] * old_x[j] + y0[j];

                            if(x[j] * x[j] + y[j] * y[j] > MAX_R)
                            {
                                int px = xi + j;
                                int py = yi;
                                for(int k = 0; k < 4; k++)
                                {
                                    int index = py * LENGTH + px;
                                    pixels[index].color = sf::Color(
                                                                    iteration * 5 % MAX_ITERATION,
                                                                    iteration * 10 % MAX_ITERATION,
                                                                    iteration * 20 % MAX_ITERATION);
                                out_mask |= (1 << j);
                                }
                            }
                        }
                    }

                    iteration++;
                }
            }
        }

        n_windows++;

        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double time_spent = seconds + microseconds * 1e-6;
        double fps = n_windows / time_spent;

        printf("fps %.3lg\n", fps);

        n_windows = 0;


        window.clear();
        window.draw(pixels);
        window.display();
    }

    return 0;
}
