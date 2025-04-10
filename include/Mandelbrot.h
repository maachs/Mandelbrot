#ifndef MANDELBROT_
#define MANDELBROT_

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sys/time.h>
#include <string.h>
#include <xmmintrin.h>
#include <assert.h>
#include <math.h>

const int BATCH_SIZE = 4;

const int MAX_WINDOWS = 10;

const int MAX_ITERATION = 256;
const int MAX_R         = 100;

const double zoomFactor = 1.1;
const double zoomStep = 1.05;

const int MAGIC_CONST_RED   = 5;
const int MAGIC_CONST_GREEN = 10;
const int MAGIC_CONST_BLUE  = 15;

struct Scale
{
    double zoom     = 1;
    double offset_x = 0;
    double offset_y = 0;
};

enum modes
{
    INTRIN = 2,
    ARRAY  = 1,
    SIMPLE = 3
};

enum size_window
{
    LENGTH = 800,
    HIGH   = 800
};

enum first_point
{
    X0 = 500,
    Y0 = 400
};

enum Zoom
{
    W_KEY = 22,
    S_KEY = 18,
    R_KEY = 17,
    R_ARROW = 72,
    L_ARROW = 71,
    U_ARROW = 73,
    D_ARROW = 74
};

void ZoomCount   (Scale* scale, char key_code);
void ResetScreen (sf::VertexArray& pixels);
void EventHandler(sf::RenderWindow& window, Scale* scale);
void CheckArgs   (const char** argv, int* mode);
void MainCycle   (Scale scale, timeval start, timeval end, sf::RenderWindow& window, int mode, sf::VertexArray& pixels);

#endif
