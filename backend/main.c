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
    
    
    char* promotors[50] = {"blackfriday"};
    // pipe 
    // fork
    // execl -> background process or new window -- probably new window is better
    Promotor promotor;
    malloc(&promotor, sizeof(Promotor))
    for (int i = 0; i < 1; i++)
    {
        pipe(promotor.fd);
        int fork_id = fork();
        if(fork_id) {
            promotor.pid = fork_id; 
            close(1);
            dup(promotor.fd[1]);
            close(promotor.fd[1]);
            close(promotor.fd[0]);

            // todo: make execl here? YESSSSSS

            // understanding select is also needed for this
            // select é efetuado no processo pai 
            exit(0);
        } else {
        
            // question -> we need one pipe per promotor? yes
            // question -> we need to use selects and have a thread or the main thread checking if some of the promotor filedescriptors
            //          -> have something to read 
            // answer -> basically we will have a do while that will have a enourmos if for each of the promotor.. since hey have a maximum of 10
            // there will be 10 stuffs

            


            // making parent process listen the commands from promotor
            // basically we need to have a infinite loop with select checking here on parent
            // while 1
            // then:
            //      if (FD_ISSET(promotor.fd[1])))
        }
    }

    

    return 1;
}
