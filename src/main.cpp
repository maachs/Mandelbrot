#include "Mandelbrot.h"
#include "ArrayVersion.h"
#include "SimpleVersion.h"

int main(const int argc, const char** argv)
{
    if(argc != 2)
    {
        printf("argc error");
        return 1;
    }
    int mode = 0;
    CheckArgs(argv, &mode);
    struct timeval start, end;

    sf::RenderWindow window(sf::VideoMode(LENGTH, HIGH), "Mandelbrot");
    sf::VertexArray pixels(sf::Points, LENGTH * HIGH);

    MainCycle(start, end, window, mode, pixels);

    return 0;
}

void MainCycle(timeval start, timeval end, sf::RenderWindow& window, int mode, sf::VertexArray& pixels)
{
    double zoom = 1;
    double offset_x = 0;
    double offset_y = 0;

    ResetScreen(pixels);

    int n_windows = 0;
    while (window.isOpen())
    {
        gettimeofday(&start, NULL);

        EventHandler(window, &zoom, &offset_x, &offset_y);

        ResetScreen(pixels);

        switch(mode)
        {
            case ARRAY:  ArrayVersion(pixels, zoom, offset_y, offset_x);
            case SIMPLE: SimpleVersion(pixels, zoom, offset_y, offset_x);
        }

        n_windows++;

        // TODO consider changing function for getting time
        // TODO rdtsc research
        //printf("wind %d\n", n_windows);
        if(n_windows == MAX_WINDOWS)
        {
            gettimeofday(&end, NULL);
            long seconds = end.tv_sec - start.tv_sec;
            long microseconds = end.tv_usec - start.tv_usec;
            double time_spent = seconds + microseconds * 1e-6;
            double fps = n_windows / time_spent;

            printf("fps %.3lg\n", fps);

            n_windows = 0;
        }
        window.clear();
        window.draw(pixels);
        window.display();
    }
}

void ZoomCount(double* zoom, double* offsetX, double* offsetY, char key_code)
{
    double moveStep = 0.1 / *zoom;
    switch (key_code)
    {
        case W_KEY:
            *zoom *= zoomStep;
            break;
        case S_KEY:
            *zoom /= zoomStep;
            break;
        case L_ARROW: *offsetX -= moveStep; break;
        case R_ARROW: *offsetX += moveStep; break;
        case U_ARROW: *offsetY -= moveStep; break;
        case D_ARROW: *offsetY += moveStep; break;
        case R_KEY:
            *zoom = 1.0;
            *offsetX = 0.0;
            *offsetY = 0.0;
            break;
    }
}

void ResetScreen(sf::VertexArray& pixels)
{
    for(int x = 0; x < LENGTH; x++)
    {
        for(int y = 0; y < HIGH; y++)
        {
            int index = x + y * LENGTH;
            pixels[index].position = sf::Vector2f(x, y);
            pixels[index].color = sf::Color::Black;
        }
    }
}

void EventHandler(sf::RenderWindow& window, double* zoom, double* offset_x, double* offset_y)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            char key_code = event.key.code;
            ZoomCount(zoom, offset_x, offset_y, key_code);
        }
    }
}

void CheckArgs(const char** argv, int* mode)
{
    if(!strcmp(argv[1], "simple"))
    {
        *mode = ARRAY;
    }
    else if(!strcmp(argv[1], "array"))
    {
        *mode = SIMPLE;
    }
    else
    {
        printf("Error: cannot define argv : %s\n", argv[1]);
        exit(0);
    }
}
