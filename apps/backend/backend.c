#include <pthread.h>
#include <stdio.h>
#include "../../shared/helpers/constants.h"
#include "../../shared/helpers/helpers.h"
#include "./commands/initializer.h"
#include "models/backend.h"

Item *load_items_from_file(char *filename) {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total = 0;
    Item *item_list;

    file = fopen(filename, "r");
    if (NULL == file){
        perror("\nError opening items file. Exiting with status");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        printf("%s", line);
    }

    for (int i = 0;i < total; i++) {
        Item current = item_list[i];
        printf("identifier: %s, bidder name: %s, current value: %d \n", current.identifier, current.bidder_name,
               current.current_value);
    }

    fclose(file);
    return item_list;
}

User *load_users_from_file(char *filename)
{
    // TODO: import library from professor and read file name, importing users

    return NULL;
}

Promotor *load_promotors_from_file(char *filename)
{
    // TODO: import library from professor and read file name, importing promotors

    return NULL;
}

Backend *bootstrap()
{
    Backend* app = malloc(sizeof(Backend));

    Config *config = get_env_variables();

    app->config = config;
    app->users = load_users_from_file("put_filename_here");
    app->promotors = load_promotors_from_file("put_filename_here");

    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, &app);
    
    // TODO: finish this logic setting up every structure for the functioning of backend,
    // this must be an adaptation of a singleton class

    return app;
}

void *command_thread_handler(void *pdata)
{
    //TODO: right now we are not doing anything with the Backend app structure
    Backend *app = (Backend *)pdata;
    //initialize the logic to receive command inputs
    command_handler_start();
}