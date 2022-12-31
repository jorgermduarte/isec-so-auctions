#include "./commands.h"
#include "../../../shared/helpers/helpers.h"
#include "../../../users_lib.h"
#include "../notifier.h"
#include "../models/backend.h"

// ======== ONLY FRONTEND COMMANDS =========
void exec_command_cash(struct Backend *app, int pid_response)
{
    if (pid_response != -1)
    { // only available for frontend apps
        printf("     > Executing the cash command \n");

        char *target_username;
        bool foundUser = false;
        printf("     > Searching for the user with pid: %d\n", pid_response);

        if (app->users != NULL)
        {
            User *current_user = app->users;
            int total_users = 0;
            while (total_users < app->config->max_users_allowed)
            {
                if (current_user->pid == pid_response)
                {
                    printf("     > Found the user! : %s \n", current_user->username);
                    foundUser = true;
                    target_username = current_user->username;
                }
                reset_heartbit_counter(app, pid_response);
                current_user++;
                total_users++;
            }

            if (foundUser)
            {
                // get the cash of the user
                int userBalance = getUserBalance(target_username);
                char message_to_send[255] = "";
                sprintf(message_to_send, "%d", userBalance);
                send_message_frontend(message_to_send, pid_response);
            }
        }
    }
}

void exec_command_verify_login(struct string_list *arguments, struct Backend *app, int pid_response)
{
    if (pid_response != -1 && arguments != NULL)
    {

        if (arguments->string != NULL && arguments->next != NULL)
        {

            char *username = arguments->string;
            char *password = arguments->next->string;
            printf("     > Executing the verify login command\n");

            int login = isUserValid(username, password);

            if (app != NULL)
            {
                // find the user in the users list and update the pid
                User *current_user = app->users;
                int total_users = 0;
                while (total_users < app->config->max_users_allowed)
                {
                    if (strcmp(current_user->username, username) == 0)
                    {
                        current_user->pid = pid_response;
                        current_user->heartbit = 20;
                        printf("     > User authenticated and defined pid on the user list: %s   -> %d\n", current_user->username, current_user->pid);
                        break;
                    }
                    current_user++;
                    total_users++;
                }
                reset_heartbit_counter(app, pid_response);
            }

            if (login == -1)
            {
                perror("      > Error validating user from file \n");
            }
            else if (login == 0)
            {
                perror("      > User doesn't exist or his password is invalid \n");
            }
            else
            {
                printf("      > User %s with password %s is logged in\n", username, password);
                send_message_frontend("LOGIN_SUCCESS", pid_response);
            }
        }
    }
}

void exec_command_time(struct Backend *app, int pid_response)
{

    if (pid_response != -1)
    {
        // Get the current time
        time_t current_time = time(NULL);
        reset_heartbit_counter(app, pid_response);

        // Display the current time in seconds
        char message_to_send[255] = "";
        sprintf(message_to_send, "%ld", current_time);
        send_message_frontend(message_to_send, pid_response);
    }
}

void exec_command_exit_frontend(struct Backend *app, int pid_response)
{

    if (pid_response != -1)
    {
        printf("     > Executing the exit operation for frontend [%d] \n", pid_response);

        for (int i = 0; i < app->config->max_promotors_allowed; i++)
        {
            if (app->frontendPids[i] == pid_response)
                app->frontendPids[i] == 0;

            if (app->users[i].pid == pid_response)
                app->users[i].pid = -1;
        }
    }
}

void exec_command_licat(struct Backend* app, int pid_response, struct string_list* arguments){
    if(pid_response != -1 && arguments != NULL && arguments->string != NULL){
        char* category = arguments->string;
        printf("     > Executing the licat command with category: %s \n", category);

        struct Item* currentItem = app->items;
        int total_items = 0;

        int file_item_size = get_file_size(app->config->f_items);

        while( total_items < file_item_size){

            //verify item category
            if(strcmp(currentItem[total_items].category,category) == 0){

                printf("    > Found item with category: %s \n", currentItem[total_items].category);

                //generate message and send to the frontend application
                struct Item item = currentItem[total_items];
                char message_to_send[255] = "";

                char currentValueString[20];
                sprintf(currentValueString, "%d", item.current_value);

                char buyNowString[20];
                sprintf(currentValueString, "%d", item.buy_now_value);

                strcat(message_to_send,item.identifier);
                strcat(message_to_send," ");
                strcat(message_to_send,item.name);
                strcat(message_to_send," ");
                strcat(message_to_send,item.category);
                strcat(message_to_send," CBID: ");
                strcat(message_to_send,currentValueString);
                strcat(message_to_send," BNV: ");
                strcat(message_to_send,buyNowString);

                send_message_frontend(message_to_send, pid_response);
            }
            total_items++;
        }

        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_litime(struct Backend* app, int pid_response, struct string_list* arguments){
    if( pid_response != -1 && arguments != NULL && arguments->string != NULL){
        if(verify_is_number(arguments->string)){

            int file_item_size = get_file_size(app->config->f_items);
            int current = 0;

            int time = atoi(arguments->string);

            while( current < file_item_size){

                struct Item* currentItem = &app->items[current];

                if(currentItem->duration <= time){ //verify if the item is still active
                    //generate message and send to the frontend application
                    char message_to_send[255] = "";

                    char currentValueString[20];
                    sprintf(currentValueString, "%d", currentItem->current_value);

                    char buyNowString[20];
                    sprintf(currentValueString, "%d", currentItem->buy_now_value);

                    strcat(message_to_send,currentItem->identifier);
                    strcat(message_to_send," ");
                    strcat(message_to_send,currentItem->name);
                    strcat(message_to_send," ");
                    strcat(message_to_send,currentItem->category);
                    strcat(message_to_send," CBID: ");
                    strcat(message_to_send,currentValueString);
                    strcat(message_to_send," BNV: ");
                    strcat(message_to_send,buyNowString);

                    send_message_frontend(message_to_send, pid_response);
                }
                current++;
            }

        }else{
            printf("     > Error: The time must be a number \n");
            send_message_frontend("  > the time input must be a number", pid_response);
        }
    }
}

// ======== ONLY BACKEND COMMANDS =========
void exec_add_money_to_user(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (arguments != NULL)
    {

        if (arguments->string != NULL)
        {

            printf("     > Executing the add money command \n");

            // find username based on the pid
            char *target_username;
            bool foundUser = false;

            User *current_user = app->users;
            int total_users = 0;
            while (total_users < app->config->max_users_allowed)
            {
                if (current_user->pid == pid_response)
                {
                    foundUser = true;
                    target_username = current_user->username;
                    break;
                }
                current_user++;
                total_users++;
            }

            char *amount = arguments->string;

            if (verify_is_number(amount) && foundUser)
            {
                // get the cash of the user
                int userBalance = getUserBalance(target_username);
                int amountToAdd = atoi(amount);
                int newBalance = userBalance + amountToAdd;

                printf("      > Adding %d to the user: %s \n", amountToAdd, target_username);
                printf("        > current balance will be: %d + %d = %d \n", userBalance, amountToAdd, newBalance);

                char message_to_send[255] = "";

                if (updateUserBalance(target_username, getUserBalance(target_username) + amountToAdd) == -1)
                {
                    sprintf(message_to_send, "error updating the balance");
                }
                else
                {
                    sprintf(message_to_send, "balance updated to %d", newBalance);
                }

                // notify the frontend
                send_message_frontend(message_to_send, pid_response);
            }
        }
    }
}

// list promoters
void exec_command_prom()
{
    printf("     > Executing the list promoters command\n");
}

// update promoters
void exec_command_reprom()
{
    printf("     > Executing the update promoters command\n");
}

// cancel promoter
void exec_command_cancel_prom(struct string_list *arguments)
{
    if (arguments != NULL)
    {
        printf("     > Executing the cancel promoter command, canceling promoter: %s\n", arguments->string);
    }
    else
    {
        printf("     > Failed to execute the cancel promoter command, please provide the exec promoter name, example: cancel <exec-promoter-name>\n");
    }
}

void exec_command_kick(struct string_list *arguments)
{
    if (arguments != NULL)
    {
        printf("     > Executing the kick user command, kicking the user: %s\n", arguments->string);
    }
    else
    {
        printf("     > Failed to execute the kick command, please provide the username, example: kick <username>\n");
    }
}

void exec_command_list_users(struct Backend *app)
{
    printf("     > Executing the list users command\n");

    FILE *file = fopen(app->config->f_users, "r");

    // Check if the file was successfully opened
    if (file == NULL)
    {
        fprintf(stderr, "Error: failed to open file '%s'\n", app->config->f_users);
        return;
    }

    printf("    >  Listing users from file: %s\n", app->config->f_users);

    // Read the file line by line
    char line[256];
    int num_users = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Parse the line to get the username, password, and budget
        char username[50];
        char password[50];
        int budget;
        int num_scanned = sscanf(line, "%s %s %d", username, password, &budget);

        if (num_scanned == 3)
        {
            printf("      >  User %d: %s %s %d\n", num_users, username, password, budget);
        }
        num_users++;
        if (num_users == app->config->max_users_allowed)
        {
            break;
        }
    }
}

// ======== BOTH FRONTEND AND BACKEND COMMANDS =========

// list all items
void exec_command_list()
{
    printf("     > Executing the list all items command\n");

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    fp = fopen("file_items.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        Item it;
        // the %20s is to avoid buffer overflows
        sscanf(line, "%20s %20s %20s %d %d %d %20s %20s", it.identifier, it.name, it.category, &it.duration, &it.current_value, &it.buy_now_value, it.seller_name, it.bidder_name);
        printf("     > %s %s %s %d %d %d %s %s\n", it.identifier, it.name, it.category, it.duration, it.current_value, it.buy_now_value, it.seller_name, it.bidder_name);
    }

    fclose(fp);
    if (line)
        free(line);
}
