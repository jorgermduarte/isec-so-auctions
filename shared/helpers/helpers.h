#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdarg.h>

void set_random();

void printToCoordinates(int x, int y, const char *format, ...);

int get_file_size(char* filename);

void bbash();
void rbash();
void creset();