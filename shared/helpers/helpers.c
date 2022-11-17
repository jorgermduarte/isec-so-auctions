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

int get_file_size(char* filename){
    FILE *fp;
    int count = 0;  
    char c;  
    fp = fopen(filename, "r");
 
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
 
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') 
            count = count + 1;
 
    fclose(fp);
    
    return count;
}

void bbash(){
    printf("\033[0;34m");
}

void rbash(){
    printf("\e[1;5;31m");
}

void creset(){
      printf("\033[0m");
}