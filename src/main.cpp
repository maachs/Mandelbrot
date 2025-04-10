#include "Mandelbrot.h"
#include "ArrayVersion.h"
#include "SimpleVersion.h"
#include "IntrinVersion.h"

int main(const int argc, const char** argv)
{
    struct timeval start, end;
    Scale scale = {};
    if(argc != 2)
    {
        printf("argc error");
        return 1;
    }
    int mode = 0;
    CheckArgs(argv, &mode);

    sf::RenderWindow window(sf::VideoMode(LENGTH, HIGH), "Mandelbrot");
    window.setVerticalSyncEnabled(false);
    sf::VertexArray pixels(sf::Points, LENGTH * HIGH);

    MainCycle(scale ,start, end, window, mode, pixels);

    return 0;
}

void MainCycle(Scale scale, timeval start, timeval end, sf::RenderWindow& window, int mode, sf::VertexArray& pixels)
{
    ResetScreen(pixels);

    int n_windows = 0;
    while (window.isOpen())
    {
        gettimeofday(&start, NULL);

        EventHandler(window, &scale);

        ResetScreen(pixels);

        switch(mode)
        {
            case ARRAY:  ArrayVersion (pixels, &scale); break;
            case SIMPLE: SimpleVersion(pixels, &scale); break;
            case INTRIN: IntrinVersion(pixels, &scale); break;
            default: printf("switch error\n"); break;
        }

        n_windows++;
        // TODO consider changing function for getting time
        // TODO rdtsc research
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

void ZoomCount(Scale* scale, char key_code)
{
    double moveStep = 0.1 / scale->zoom;
    switch (key_code)
    {
        case W_KEY:
            scale->zoom *= zoomStep;
            break;
        case S_KEY:
            scale->zoom /= zoomStep;
            break;
        case L_ARROW: scale->offset_x -= moveStep; break;
        case R_ARROW: scale->offset_x += moveStep; break;
        case U_ARROW: scale->offset_y -= moveStep; break;
        case D_ARROW: scale->offset_y += moveStep; break;
        case R_KEY:
            scale->zoom = 1.0;
            scale->offset_x = 0.0;
            scale->offset_y = 0.0;
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

void EventHandler(sf::RenderWindow& window, Scale* scale)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            char key_code = event.key.code;
            ZoomCount(scale, key_code);
        }
    }
}

void CheckArgs(const char** argv, int* mode)
{
    if(!strcmp(argv[1], "simple"))
    {
        *mode = SIMPLE;
    }
    else if(!strcmp(argv[1], "array"))
    {
        *mode = ARRAY;
    }
    else if(!strcmp(argv[1], "simd"))
    {
        *mode = INTRIN;
    }
    else
    {
        printf("Error: cannot define argv : %s\n", argv[1]);
        exit(0);
    }
}
