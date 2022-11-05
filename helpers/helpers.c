#include "helpers.h"


void set_random()
{
    srand(time(NULL));
}

bool contains( char *str[], char* substr[] )
{
    bool isPresent = false;
    for (int i = 0; str[i] != '\0'; i++) {
        isPresent = false;
        for (int j = 0; substr[j] != '\0'; j++) {
            if (str[i + j] != substr[j]) {
                isPresent = false;
                break;
            }
            isPresent = true;
        }
        if (isPresent) break;
    }

    return isPresent;
}