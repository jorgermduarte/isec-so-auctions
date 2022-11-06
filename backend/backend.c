#include "../models/backend.h"
#include "../helpers/constants.h"
#include "../helpers/helpers.h"
#include "./commands/commands.h"

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
    Backend backend;
    malloc(&backend, sizeof(Backend));

    Config config = get_env_variables();

    backend->config = config;
    backend->users = load_users_from_file("put_filename_here");
    backend->promotors = load_promotors_from_file("put_filename_here");

    // TODO: finish this logic setting up every structure for the functioning of backend,
    // this must be an adaptation of a singleton class
}

void *command_thread_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;
    char command[CMD_BUF_SIZE];

    while (1)
    {
        scanf("$> %s", command);
        int cmdLen = strlen(command);

        if (contains(command, "list")) {
            list_all_items(app, command);
        } else if (contains(command, "users")) {
            list_users(app, command);
        } else if (contains(command, "kick")) {
            kick_user(app, command);
        } else if (contains(command, "prom")) {
            list_promotors(app, command);
        } else if (contains(command, "reprom")) {
            update_promotors(app, command);
        } else if (contains(command, "cancel")) {
            cancel_promotor(app, command);
        } else if (contains(command, "close")) {
            close_platform(app, command);
        } else {
            printf("Command unavailable \n");
        }
        
    }
}