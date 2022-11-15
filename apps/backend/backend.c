#include <pthread.h>
#include "../../shared/helpers/constants.h"
#include "../../shared/helpers/helpers.h"
#include "./commands/initializer.h"
#include "models/backend.h"

Item *load_items_from_file(char *filename)
{
    // TODO: import library from professor and read file name, importing items

    return NULL;
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
    Backend *app = malloc(sizeof(Backend));

    Config *config = get_env_variables();

    app->config = config;
    app->users = load_users_from_file("put_filename_here");
    app->promotors = load_promotors_from_file("put_filename_here");

    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, &app);
    
    // TODO: finish this logic setting up every structure for the functioning of backend,
    // this must be an adaptation of a singleton class
}

void *command_thread_handler(void *pdata)
{
    //TODO: right now we are not doing anything with the Backend app structure
    Backend *app = (Backend *)pdata;
    //initialize the logic to receive command inputs
    command_handler_start();
}