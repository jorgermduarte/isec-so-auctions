#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands/initializer.h"
#include "string.h"
#include "models/backend.h"

int main(int argc, char *argv[])
{
    // TODO: validate if this works propertly because the .h header file is in a different directory
    // from the .c file. don't really tested this but it could work. if not we need to move .h file 
    // to the backend directory
    Backend* app = bootstrap();
    
    
    char* promotors[50] = {"blackfriday"};
    // pipe 
    // fork
    // execl -> background process or new window -- probably new window is better
    Promotor* promotor = malloc(sizeof(Promotor));
    

    pipe(promotor->fd);
    int fork_id = fork();
    if(fork_id == 0) {
        close(1);
        dup(promotor->fd[1]);
        close(promotor->fd[1]);
        close(promotor->fd[0]);

        execl("promotor", "promotor", (char*)NULL);

        exit(0);
    } else if (fork_id > 0) {
        promotor->pid = fork_id;
        close(promotor->fd[1]);
        char buffer[20] = "\0";
        while(1) {
            int size = read(promotor->fd[0], buffer, sizeof(buffer));
            
            if (strstr(buffer, "exit"))
            {
                    printf("\nExiting....");
                    exit(0);
            }
                
            printf("\n[Promoter - p%d] sended the following message : %s", promotor->pid, buffer);    
            memset(buffer, 0, sizeof(buffer));
        }
        

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

    return 1;
}
