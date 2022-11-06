#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../config/config.h"
#include "../models/backend.h"
#include "../models/promotor.h"

Backend* app;

int main(int argc, char *argv[])
{

    // TODO: validate if this works propertly because the .h header file is in a different directory
    // from the .c file. don't really tested this but it could work. if not we need to move .h file 
    // to the backend directory
    app = bootstrap();
    
    // temp promotor array
    char* promotors[50] = {"blackfriday"};
    // TODO: loop through promotors available and create:
    // pipe 
    // fork
    // execl -> background process or new window -- probably new window is better
    Promotor promotor;
    for (int i = 0; i < 1; i++)
    {
        pipe(promotor.fd);
        int fork_id = fork()
        if(fork_id) {
            promotor.pid = fork_id; 
            close(1);
            dup(promotor.fd[1]);
            close(promotor.fd[1]);
            close(promotor.fd[1]);
            close(promotor.fd[0]);

            // todo: make execl here? 
            // understanding select is also needed for this
        } else {
            // making parent process listen the commands from promotor
        }
            
    }

    

    return 1;
}
