#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

void set_random();

void printToCoordinates(int x, int y, const char *format, ...);

void set_attribute(char *attribute, char* value);