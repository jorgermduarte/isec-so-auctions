#include <pthread.h>
#include <string.h>
#include "../helpers/constants.h"
#include "../helpers/helpers.h"
#include "../commands/commands.h"

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
    Backend *app = (Backend *)pdata;
    char command[CMD_BUF_SIZE];

    while (1)
    {
        //printToCoordinates(200, 200, "%s", "$> ");
        printf("$> ");
        scanf("%s", command);
        int cmdLen = strlen(command);

        if (strstr((char*)command, "list")) {
            list_all_items(app, command);
        } else if (strstr((char*)command, "users")) {
            list_users(app, command);
        } else if (strstr((char*)command, "kick")) {
            kick_user(app, command);
        } else if (strstr((char*)command, "prom")) {
            list_promotors(app, command);
        } else if (strstr((char*)command, "reprom")) {
            update_promotors(app, command);
        } else if (strstr((char*)command, "cancel")) {
            cancel_promotor(app, command);
        } else if (strstr((char*)command, "close")) {
            close_platform(app, command);
        } else {
            printf("Command unavailable \n");
        }
        fflush(stdout);
    }
}