#include "helpers.h"


void set_random()
{
    srand(time(NULL));
}

void printToCoordinates(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", x, y);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}
