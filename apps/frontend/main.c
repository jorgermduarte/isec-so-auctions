#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "commands/initializer.h"
#include "../../shared/helpers/pipes.h"
#include "../../shared/models/communication.h"

void backend_communication_fifo_initializer(char* pipe_name)
{
    //create the named pipe for the frontend to receive responses from the backend
    if (mkfifo(pipe_name, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            perror("\n[ERR] FRONTEND FIFO ALREADY EXISTS");
        }else{
            perror("\n[ERR] ERROR WHILE CREATING FIFO");
            exit(1);
        }
    }
}

void* backend_communication_receiver_handler(void* data)
{
    char* pipe_name = (char*)data;
    Message msg;
    printf("[INFO] FRONTEND COMMUNICATION RECEIVER HANDLER STARTED FOR PIPE: %s\n",pipe_name);

    while(1){
        int fd_bfifo = open(pipe_name,  O_RDONLY | O_NONBLOCK);
        if(fd_bfifo == -1){
            //printf(" > Something went wrong receiving the message from a backend application \n");
        }else{
            int read_size = sizeof(msg) + 1;
            int size = read(fd_bfifo, &msg, read_size);
            if(size > 0) {
                printf(" > [RECEIVED RESPONSE FROM BACKEND]: %s\n", msg.response.result);
            }
            close(fd_bfifo);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Please provide the username and password when initializing the client(frontend) application.\n");
        printf("Example: ./client manuel password_do_manuel\n");
    } else {
        printf("======================= USER PROVIDED ======================== \n");
        char *username = argv[1];
        char *password = argv[2];
        printf("username: %s\n", username);
        printf("password: *******\n");
        printf("========================== COMMANDS ========================== \n");


        //initialize the commands receiver fifo from the backend application
        char pipe_name_result[100] = "";

        pid_t pid = getpid();
        char pid_string[10];
        sprintf(pid_string, "%d", pid); // integer to string
        char* pipe_name = FRONTEND_FIFO_NAME_BASE;
        strcat(pipe_name_result,FRONTEND_FIFO_NAME_BASE);
        strcat(pipe_name_result,pid_string);
        printf(" > FRONTEND PIPE NAME DEFINED: %s\n", pipe_name_result);

        backend_communication_fifo_initializer(pipe_name);

        //create a thread to receive the responses from the backend
        pthread_t pthread_frontend_responses;
        pthread_create(&pthread_frontend_responses, NULL, backend_communication_receiver_handler, (void*)pipe_name_result);
        command_handler_start();


    }

    return 0;
}
