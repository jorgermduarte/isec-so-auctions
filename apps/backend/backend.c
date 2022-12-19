#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "models/backend.h"
#include "./commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "../../shared/helpers/constants.h"
#include "../../shared/models/communication.h"
#include "../../shared/helpers/pipes.h"

Backend *bootstrap()
{
    // Create main structures for the backend to work
    Backend *app = malloc(sizeof(Backend));

    //define config values
    Config *config = get_env_variables();
    app->config = config;

    //initialize the commands receiver fifo from the frontend applications
    frontend_communication_fifo_initializer();

    // Instantiate threads
    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, &app);
    pthread_create(&app->threads.pthread_frontend_requests, NULL, frontend_communication_receiver_handler, &app);

    return app;
}

void *command_thread_handler(void *pdata)
{
    // TODO: right now we are not doing anything with the Backend app structure
    Backend *app = (Backend *)pdata;
    // initialize the logic to receive command inputs
    command_handler_start();
}

void frontend_communication_fifo_initializer()
{
    //create the named pipe for the frontend to send requests and the backend read them
    if (mkfifo(BACKEND_FIFO_NAME, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            perror("\n[ERR] FIFO ALREADY EXISTS");
        }else{
            perror("\n[ERR] ERROR WHILE CREATING FIFO");
            exit(1);
        }
    }
}

void *frontend_communication_receiver_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;
    Message msg;

    while(1){
        int fd_bfifo = open(BACKEND_FIFO_NAME, O_RDONLY);
        if(fd_bfifo == -1){
            printf(" > Something went wrong receiving the message from a frontend application \n");
        }else{
            int size = read(fd_bfifo, &msg, sizeof(msg));
            if(size > 0) {
                printf(" > [FRONTEND %d] [REQUESTED] %s", msg.pid, msg.request.arguments);
            }
            close(fd_bfifo);
        }
    }
}