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

void clean_item(Item* item){
    free(item->bidder_name);
    free(item->seller_name);
    free(item->category);
    free(item->identifier);
    free(item->name);
    free(item);
}

//TODO: Not sure if it is right (verify later)
void clean_list_item(Item* item){
    while(item != NULL){
        clean_item(item);
        item++;
    }
}