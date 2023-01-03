#include "helpers.h"
#include <string.h>

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

int get_file_size(char *filename)
{
    FILE *fp;
    char c;
    fp = fopen(filename, "r");

    char buf[TMP_MAX];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, TMP_MAX, fp);
        if (ferror(fp))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(fp))
            break;
    }

    return counter;
}

void bbash()
{
    printf("\033[0;34m");
}

void rbash()
{
    printf("\e[0;31m");
}

void creset()
{
    printf("\033[0m");
}