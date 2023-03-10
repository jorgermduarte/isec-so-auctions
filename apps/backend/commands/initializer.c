#include <pthread.h>
#include <unistd.h>
#include "initializer.h"
#include "../../../shared/helpers/helpers.h"

int command_try_execution(char *command, struct string_list *arguments, int pid_response, Backend *app)
{
    //printf("    > Verifying command existence\n");
    int exit = 0;

    if (command_validate_name(command, "list"))
    { // available for backend and frontend applications
        exec_command_list(app,pid_response);
    }
    else if (command_validate_name(command, "login") && pid_response != -1 && arguments != NULL)
    { // only available for frontend apps
        exec_command_verify_login(arguments, app, pid_response);
    }
    else if (command_validate_name(command, "cash") && pid_response != -1)
    { // only available for frontend applications
        exec_command_cash(app, pid_response);
    }
    else if (command_validate_name(command, "add") && pid_response != -1 && arguments != NULL)
    { // only available for frontend applications
        exec_add_money_to_user(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "time") && pid_response != -1)
    { // only available for frontend applications
        exec_command_time(app, pid_response);
    }
    else if (command_validate_name(command, "licat") && pid_response != -1)
    { // only available for frontend applications
        exec_command_licat(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "litime") && pid_response != -1)
    { // only available for frontend applications
        exec_command_litime(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "lival") && pid_response != -1)
    { // only available for frontend applications
        exec_command_lival(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "lisel") && pid_response != -1)
    { // only available for frontend applications
        exec_command_lisel(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "sell") && pid_response != -1)
    { // only available for frontend applications
        exec_command_sell(app, pid_response, arguments);
    }
    else if (command_validate_name(command, "buy") && pid_response != -1)
    {  // only available for frontend applications
        exec_command_buy(app, pid_response, arguments);
    }

    else if (command_validate_name(command, "users") && pid_response == -1 ) //only available to the backend
    {
        exec_command_list_users(app);
    }
    else if (command_validate_name(command, "kick"))
    {
        exec_command_kick(app, arguments);
    }
    else if (command_validate_name(command, "prom") && pid_response == -1 ) // only available to the backend
    {
        exec_command_prom(app);
    }
    else if (command_validate_name(command, "reprom") && pid_response == -1)  // only available to the backend
    {
        exec_command_reprom(app);
    }
    else if (command_validate_name(command, "cancel"))
    {
        exec_command_cancel_prom(app,arguments);
    }
    else if (command_validate_name(command, "close"))
    {
        printf("     > closing the application...\n");
        exit = 1;
    }
    else if (command_validate_name(command, "exit") && pid_response != -1)
    {
        exec_command_exit_frontend(app, pid_response);
    }
    else
    {
        printf("     > Couldn't find any command for: %s\n", command);
    }
    return exit;
}

void command_handler_start(Backend *app)
{
    creset();
    size_t bufsize = 255;
    char buffer[bufsize];

    if (app->threads.running == 0)
    {
        pthread_exit(0);
    }

    //printf(" > [INFO] WE ARE LISTENING TO ANY COMMAND:\n");
    printf("\n> ");
    fgets(buffer, bufsize, stdin);

    if (app->threads.running == 0)
    {
        pthread_exit(0);
    }

    printf(" > Command received: %s", buffer);

    size_t len = strlen(buffer);

    if (len > 1)
    {
        // remove a possible added \n
        if (buffer[len - 1] == '\n')
        { // FAILS when len == 0
            buffer[len - 1] = '\0';
        }

        struct string_list *arguments = get_command_arguments(buffer);

        // command_arguments_display(arguments);
        int close_app = command_try_execution(arguments->string, arguments->next, -1, app);
        if (!close_app)
        {
            // clean the arguments memory allocated to avoid memory leaks
            clean_linked_list(arguments);
            command_handler_start(app);
        }
        else
        {
            kill(getpid(), SIGINT);
        }
    }
    else
    {
        printf("> Invalid command provided, please try again..\n");
        command_handler_start(app);
    }
}
