#include <signal.h>
#include <unistd.h>
#include "./commands.h"
#include "../models/notifier.h"

void exec_command_list()
{
    //printf("     > Executing the list command\n");
    send_message_backend("list");
}

void exec_command_licat(struct string_list *arguments)
{
    if (arguments != NULL)
    {
        //printf("     > Executing the licat command, with the category: %s\n", arguments->string);

        char message[256] = "licat ";
        strcat(message, arguments->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the licat command, please provide the category name, example: licat category-name\n");
    }
}

void exec_command_lisel(struct string_list *arguments)
{
    if (arguments != NULL)
    {
        //printf("     > Executing the lisel command, with the seller name: %s\n", arguments->string);

        char message[256] = "lisel ";
        strcat(message, arguments->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the lisel command, please provide the category name, example: lisel seller-name\n");
    }
}

void exec_command_lival(struct string_list *arguments)
{
    if (arguments != NULL && verify_is_number(arguments->string))
    {
        //printf("     > Executing the lival command, with the value: %s\n", arguments->string);

        char message[256] = "lival ";
        strcat(message, arguments->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the lival command, please provide a value, example: lival 30\n");
    }
}

void exec_command_litime(struct string_list *arguments)
{
    if (arguments != NULL && verify_is_number(arguments->string))
    {
        //printf("     > Executing the litime command, with the value: %s\n", arguments->string);
        char message[256] = "litime ";
        strcat(message, arguments->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the litime command, please provide a value, example: litime 30\n");
    }
}

void exec_command_time()
{
    //printf("     > Executing the time command\n");
    send_message_backend("time");
}

void exec_command_buy(struct string_list *arguments)
{
    if (arguments != NULL && arguments->next != NULL && verify_is_number(arguments->string) &&
        verify_is_number(arguments->next->string))
    {
        printf("Executing the buy command, with the values: id: %s , value: %s\n", arguments->string,       arguments->next->string);

        char message[256] = "buy ";
        strcat(message, arguments->string);
        strcat(message, " ");
        strcat(message, arguments->next->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the buy command, please provide two values, example: buy <id> <value>\n");
    }
}

void exec_command_cash()
{
    //printf("     > Executing the cash command\n");
    char message[256] = "cash";
    send_message_backend(message);
}

void exec_command_add(struct string_list *arguments)
{
    if (arguments != NULL && arguments->string != NULL && verify_is_number(arguments->string))
    {
        //printf("     > Executing the add command, with the value: %s\n", arguments->string);

        char message[256] = "add ";
        strcat(message, arguments->string);
        send_message_backend(message);
    }
    else
    {
        printf("     > Failed to execute the add command, please provide a value, example: add 30\n");
    }
}

void exec_command_exit()
{
    pid_t pid = getpid();
    char message[256] = "exit";
    //printf("     > Executing the order to %s the frontend with pid %d\n", message, pid);
    send_message_backend(message);

    kill(pid, SIGINT);
}

void exec_command_sell(struct string_list *arguments)
{
    if( arguments != NULL && arguments->next != NULL && arguments->next->next != NULL && arguments->next->next->next != NULL && arguments->next->next->next->next != NULL){

        if(verify_is_number(arguments->next->next->string) && verify_is_number(arguments->next->next->next->string) && verify_is_number(arguments->next->next->next->next->string) ){
            printf("     > Executing the sell command, with the values: %s %s %s %s\n", arguments->string, arguments->next->string, arguments->next->next->string, arguments->next->next->next->string);

            char message[256] = "sell ";
            strcat(message, arguments->string); // name
            strcat(message, " ");
            strcat(message, arguments->next->string); // category
            strcat(message, " ");
            strcat(message, arguments->next->next->string); // valor
            strcat(message, " ");
            strcat(message, arguments->next->next->next->string); // buy now
            strcat(message, " ");
            strcat(message, arguments->next->next->next->next->string); // time

            send_message_backend(message);
        }
        else{
            printf("     > Failed to execute the sell command, please provide four values, example: sell <name> <category> <price> <buynow> <time>\n");
        }

    }else{
        printf("     > Failed to execute the sell command, please provide four values, example: sell <name> <category> <price> <buynow> <time>\n");
    }
}