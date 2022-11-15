#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdarg.h>
#include "../../apps/backend/models/item.h"

void set_random();

void printToCoordinates(int x, int y, const char *format, ...);

void clean_item(Item* item);

//TODO: implement the clean up of the dynamic list of items
void clean_list_item(Item* item);