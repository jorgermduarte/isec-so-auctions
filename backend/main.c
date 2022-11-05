#include <stdio.h>
#include "../config/config.h"
#include "../models/backend.h"

Backend* app;

int main(int argc, char *argv[])
{

    // todo: validate if this works propertly because the .h header file is in a different directory
    // from the .c file. don't really tested this but it could work. if not we need to move .h file 
    // to the backend directory
    app = bootstrap();

    // todo: loop through promotors available and create:
    // pipe 
    // fork
    // execl -> background process or new window -- probably new window is better

    return 1;
}
