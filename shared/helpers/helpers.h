#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdarg.h>

/**
 * Activates random numbers
 */
void set_random();

/**
 * Prints a given string with arguments in the specified coordinates
 *
 * @param x
 * @param y
 * @param format
 * @param ...
 */
void printToCoordinates(int x, int y, const char *format, ...);

/**
 * Calculates the number of lines present in a file
 *
 * @param filename
 * @return int lines_count
 */
int get_file_size(char* filename);

/**
 * Sets the text color of the bash blue
 */
void bbash();

/**
 * Sets the text color of the bash red
 */
void rbash();

/**
 * Resets the colors of the bash to the default ones
 */
void creset();