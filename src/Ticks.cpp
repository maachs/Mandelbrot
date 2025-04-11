#include "Ticks.h"

void AddTicks(Ticks* ticks)
{
    assert(ticks);

    if(ticks->actual_count == ticks->actual_max)
    {
        printf("to real\n");
        ticks->ticks_array = (uint64_t*) realloc(ticks->ticks_array, (ticks->actual_max + STEP_SIZE) * sizeof(uint64_t));
        if(ticks->ticks_array == NULL)
        {
            printf("realloc error\n");
            TicksDtor(ticks);
            exit(0);
        }
        ticks->actual_max += STEP_SIZE;
    }

    ticks->ticks_array[ticks->actual_count] = ticks->end - ticks->start;
    ticks->actual_count++;

}

void TicksDtor(Ticks* ticks)
{
    assert(ticks);
    free(ticks->ticks_array);
    ticks->ticks_array = NULL;
}

void PrintAvgTicks(Ticks* ticks)
{
    assert(ticks);

    if(ticks->actual_count == 0)
    {
        printf("No ticks recorded\n");
        return;
    }
    uint64_t sum = 0;
    for(int ticks_i = 0; ticks_i < ticks->actual_count; ticks_i++)
    {
        sum += ticks->ticks_array[ticks_i];
    }

    uint64_t avg = sum / ticks->actual_count;
    printf("AVG ticks: %ld\n\n", avg);
    printf("sum %ld\ncnt %d\n", sum, ticks->actual_count);

    TicksDtor(ticks);
}

