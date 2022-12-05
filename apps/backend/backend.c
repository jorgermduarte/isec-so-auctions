#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "models/backend.h"
#include "./commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "../../shared/helpers/constants.h"
#include "../../shared/models/communication.h"

// TODO: i think we should move the items list to a command !
//  it was necessary for the first meta!
void load_items_from_file(char *filename, Item *items)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        Item it;
        // the %20s is to avoid buffer overflows
        sscanf(line, "%20s %20s %20s %d %d %d %20s %20s", it.identifier, it.name, it.category, &it.duration, &it.current_value, &it.buy_now_value, it.seller_name, it.bidder_name);
        items[i++] = it;
    }

    fclose(fp);
    if (line)
        free(line);
}

User *load_users_from_file(char *filename)
{
    // TODO: import library from professor and read file name, importing users

    return NULL;
}

Promotor *load_promoters_from_file(char *filename)
{
    // TODO: import library from professor and read file name, importing promoters

    return NULL;
}

Backend *bootstrap()
{
    // Create main structures for the backend to work
    Backend *app = malloc(sizeof(Backend));
    Config *config = get_env_variables();
    app->fifo_b = copen_fifo_backend();
    int file_item_size = get_file_size(app->config->f_items);
    app->config = config;

    // Read data from files
    Item items[file_item_size];
    load_items_from_file(app->config->f_items, &items[0]);
    app->items = items;

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

int copen_fifo_backend()
{
    if (mkfifo(BNAMEFIFO, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            perror("\n[ERR] FIFO ALREADY EXISTS");
        }
        perror("\n[ERR] ERROR WHILE CREATING FIFO");
        exit(1);
    }

    // int fd_bfifo = open(BNAMEFIFO, O_RDWR);    
    int fd_bfifo = open(BNAMEFIFO, O_RDONLY);
    if(fd_bfifo == -1){
       perror("\n[ERR] ERROR WHILE OPENING FIFO");
       exit(1);
    }

    return fd_bfifo;
}

void *frontend_communication_receiver_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;
    Message msg;

    while(1){
        int size = read(app->fifo_b, &msg, sizeof(msg));
        if(size > 0) {
            printf("[FRONTEND %d] [REQUESTED] %s", msg.pid, msg.request.arguments);
        }
    }
}