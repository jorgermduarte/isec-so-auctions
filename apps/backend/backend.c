#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "models/backend.h"
#include "./commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "../../shared/helpers/constants.h"


//TODO: i think we should move the items list to a command !
// it was necessary for the first meta!
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
        //the %20s is to avoid buffer overflows
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
    Backend *app = malloc(sizeof(Backend));
    Config *config = get_env_variables();
    app->config = config;

    int file_item_size = get_file_size(app->config->f_items);
    Item items[file_item_size];

    load_items_from_file(app->config->f_items, &items[0]);
    app->items = items;
    
    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, &app);

    return app;
}

void *command_thread_handler(void *pdata)
{
    // TODO: right now we are not doing anything with the Backend app structure
    Backend *app = (Backend *)pdata;
    // initialize the logic to receive command inputs
    command_handler_start();
}