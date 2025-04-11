#ifndef TICKS_
#define TICKS_

#include "Mandelbrot.h"

const int MAX_TICKS_SIZE = 20;
const int STEP_SIZE = 10;


void AddTicks     (Ticks* ticks);
void TicksDtor    (Ticks* ticks);
void PrintAvgTicks(Ticks* ticks);

#endif
