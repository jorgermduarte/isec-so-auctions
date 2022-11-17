#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "string.h"
#include "models/backend.h"

int main(int argc, char *argv[])
{
    Backend* app = bootstrap();
    int msg_count = 0;
    
    // pipe 
    // fork
    // execl -> background process

    Promotor* promotor = malloc(sizeof(Promotor));
    
    pipe(promotor->fd);
    int fork_id = fork();
    if(fork_id == 0) {
        close(1);
        dup(promotor->fd[1]);
        close(promotor->fd[1]);
        close(promotor->fd[0]);

        execl("promotor_oficial", "promotor_oficial", (char*)NULL);

        exit(0);
    } else if (fork_id > 0) {
        promotor->pid = fork_id;
        close(promotor->fd[1]);
        char buffer[20] = "\0";
        while(1) {
            int size = read(promotor->fd[0], buffer, sizeof(buffer));
            
            if (strstr(buffer, "exit")) {
                    printf("\nExiting....");
                    exit(0);
            }

            if(size != 0){
                rbash();
                printf("\n[Promoter - p%d] sended the following message : %s\033[0m", promotor->pid, buffer);    
                creset();
                msg_count++;
            }
            memset(buffer, 0, sizeof(buffer));
            
            if (msg_count == 5)
            {
                printf("    > Sending SIGUSR1 signal to promoter\n");
                union sigval val;
                val.sival_int = 0;
                
                sigqueue(promotor->pid, SIGUSR1, val); 
                // sleep 10 so we can see the dead promotor in jobs
                // exit right after
                sleep(10);         
                printf("    > Exiting\n");
                exit(0);      
            }
            
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
