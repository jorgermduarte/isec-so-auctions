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

        reset_heartbit_counter(app, pid_response);
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
        reset_heartbit_counter(app, pid_response);
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

void exec_command_licat(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (pid_response != -1 && arguments != NULL && arguments->string != NULL)
    {
        char *category = arguments->string;
        printf("     > Executing the licat command with category: %s \n", category);

        struct Item *currentItem = app->items;
        int total_items = 0;

        int file_item_size = get_file_size(app->config->f_items);

        while (total_items < file_item_size)
        {

            // verify item category
            if (strcmp(currentItem[total_items].category, category) == 0)
            {

                printf("    > Found item with category: %s \n", currentItem[total_items].category);

                // generate message and send to the frontend application
                struct Item item = currentItem[total_items];
                char message_to_send[255] = "";

                char currentValueString[20];
                sprintf(currentValueString, "%d", item.current_value);

                char buyNowString[20];
                sprintf(currentValueString, "%d", item.buy_now_value);

                strcat(message_to_send, item.identifier);
                strcat(message_to_send, " ");
                strcat(message_to_send, item.name);
                strcat(message_to_send, " ");
                strcat(message_to_send, item.category);
                strcat(message_to_send, " CBID: ");
                strcat(message_to_send, currentValueString);
                strcat(message_to_send, " BNV: ");
                strcat(message_to_send, buyNowString);

                send_message_frontend(message_to_send, pid_response);
            }
            total_items++;
        }

        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_litime(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (pid_response != -1 && arguments != NULL && arguments->string != NULL)
    {
        if (verify_is_number(arguments->string))
        {

            int file_item_size = get_file_size(app->config->f_items);
            int current = 0;

            int time = atoi(arguments->string);

            while (current < file_item_size)
            {

                struct Item *currentItem = &app->items[current];

                if (currentItem->duration <= time)
                { // verify if the item is still active
                    // generate message and send to the frontend application
                    char message_to_send[255] = "";

                    char currentValueString[20];
                    sprintf(currentValueString, "%d", currentItem->current_value);

                    char buyNowString[20];
                    sprintf(currentValueString, "%d", currentItem->buy_now_value);

                    strcat(message_to_send, currentItem->identifier);
                    strcat(message_to_send, " ");
                    strcat(message_to_send, currentItem->name);
                    strcat(message_to_send, " ");
                    strcat(message_to_send, currentItem->category);
                    strcat(message_to_send, " CBID: ");
                    strcat(message_to_send, currentValueString);
                    strcat(message_to_send, " BNV: ");
                    strcat(message_to_send, buyNowString);

                    send_message_frontend(message_to_send, pid_response);
                }
                current++;
            }
        }
        else
        {
            printf("     > Error: The time must be a number \n");
            send_message_frontend("  > the time input must be a number", pid_response);
        }
        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_lival(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (pid_response != -1 && arguments != NULL && arguments->string != NULL)
    {
        if (verify_is_number(arguments->string))
        {

            int file_item_size = get_file_size(app->config->f_items);
            int current = 0;

            int price = atoi(arguments->string);

            while (current < file_item_size)
            {
                struct Item *currentItem = &app->items[current];

                if (currentItem->current_value <= price || currentItem->buy_now_value <= price)
                { // verify if the item price is lower than the input
                    // generate message and send to the frontend application
                    char message_to_send[255] = "";

                    char currentValueString[20];
                    sprintf(currentValueString, "%d", currentItem->current_value);

                    char buyNowString[20];
                    sprintf(currentValueString, "%d", currentItem->buy_now_value);

                    strcat(message_to_send, currentItem->identifier);
                    strcat(message_to_send, " ");
                    strcat(message_to_send, currentItem->name);
                    strcat(message_to_send, " ");
                    strcat(message_to_send, currentItem->category);
                    strcat(message_to_send, " CBID: ");
                    strcat(message_to_send, currentValueString);
                    strcat(message_to_send, " BNV: ");
                    strcat(message_to_send, buyNowString);

                    send_message_frontend(message_to_send, pid_response);
                }
                current++;
            }
        }
        else
        {
            printf("     > Error: The value must be a number \n");
            send_message_frontend("  > the value input must be a number", pid_response);
        }
        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_lisel(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (pid_response != -1 && arguments != NULL && arguments->string != NULL)
    {
        int file_item_size = get_file_size(app->config->f_items);
        int current = 0;

        char *sellerName = arguments->string;

        while (current < file_item_size)
        {
            struct Item *currentItem = &app->items[current];

            if (strcmp(sellerName, currentItem->seller_name) == 0)
            { // verify if the item is from the seller
                // generate message and send to the frontend application
                char message_to_send[255] = "";

                char currentValueString[20];
                sprintf(currentValueString, "%d", currentItem->current_value);

                char buyNowString[20];
                sprintf(currentValueString, "%d", currentItem->buy_now_value);

                strcat(message_to_send, currentItem->identifier);
                strcat(message_to_send, " ");
                strcat(message_to_send, currentItem->name);
                strcat(message_to_send, " ");
                strcat(message_to_send, currentItem->category);
                strcat(message_to_send, " CBID: ");
                strcat(message_to_send, currentValueString);
                strcat(message_to_send, " BNV: ");
                strcat(message_to_send, buyNowString);

                send_message_frontend(message_to_send, pid_response);
            }
            current++;
        }
        reset_heartbit_counter(app, pid_response);
    }
}

void exec_add_money_to_user(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (arguments != NULL && pid_response != -1 && arguments->string != NULL)
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
        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_sell(struct Backend *app, int pid_response, struct string_list *arguments)
{
    if (arguments != NULL && pid_response != -1 && arguments->string != NULL)
    {
        printf("     > Starting new auction for the item %s during %d \n", arguments->string, atoi(arguments->next->next->next->next->string));

        User logged_in_user = get_logged_in_user(app, pid_response, "");

        Item item_for_sale = {
            .bidder_name = "",
            .seller_name = "",
            .buy_now_value = atoi(arguments->next->next->next->string),
            .category = "",
            .name = "",
            .current_value = atoi(arguments->next->next->string),
            .duration = atoi(arguments->next->next->next->next->string),
            .identifier = "",
            .active = 1};

        strcpy(item_for_sale.category, arguments->next->string);
        strcpy(item_for_sale.name, arguments->string);
        strcpy(item_for_sale.seller_name, logged_in_user.username);

        char message_to_send[255] = "";
        int added = 0;
        for (int i = 0; i < app->config->max_auctions_active; i++)
        {
            if (app->items[i].active != 1)
            {
                app->items[i] = item_for_sale;
                added = 1;
                sprintf(message_to_send, "Auction for %s is ongoing. Seller is %s and the base price is %d€\n", item_for_sale.name, item_for_sale.seller_name, item_for_sale.current_value);
                break;
            }
        }

        if (added == 0)
            sprintf(message_to_send, "Auction for %s did not start because of the maximum of auctions reached.\n", item_for_sale.name);

        for (int i = 0; i < app->config->max_users_allowed; i++)
        {
            if(app->users[i].pid != -1 && app->users[i].pid != 0){
                send_message_frontend(message_to_send, app->users[i].pid);
            }
        }

        reset_heartbit_counter(app, pid_response);
    }
}

void exec_command_buy(struct Backend* app, int pid_response, struct string_list *arguments){
    if(pid_response != -1 && arguments != NULL && arguments->string != NULL && arguments->next != NULL && arguments->next->string != NULL){
        // a frontend application is trying to buy an item
        char* amount = arguments->next->string;
        if(verify_is_number(amount) && verify_is_number(arguments->string)){

            int itemIdentifier = atoi(arguments->string);
            int amountNumber = atoi(amount);

            // find the item
            int item_index = 0;
            bool foundItem = false;
            while( item_index < app->config->max_auctions_active){
                if( app->items[item_index].active == 1 && app->items[item_index].unique_id == itemIdentifier){
                    foundItem = true;
                    // found the item
                    User* loggedUser = find_user_by_pid(app, pid_response);

                    if(loggedUser != NULL){

                        // verify if the user has enough money
                        int userBalance = getUserBalance(loggedUser->username);

                        if(userBalance >= amountNumber && amountNumber >= app->items[item_index].current_value){

                            // place the bid in the item
                            app->items[item_index].current_value = amountNumber;
                            strcpy(app->items[item_index].bidder_name, loggedUser->username);

                            if(app->items[item_index].current_value >= app->items[item_index].buy_now_value){
                                // the user bought the item
                                app->items[item_index].active = 0;

                                // update the user balance based on the buy_now_value
                                updateUserBalance(loggedUser->username, userBalance - app->items[item_index].buy_now_value);

                                // notify the frontend
                                char message_to_send[255] = "";
                                sprintf(message_to_send, "You bought the item %s for %d", app->items[item_index].name, amountNumber);
                                send_message_frontend(message_to_send, pid_response);

                                // notify every frontend that the item was sold expect for the user that bought the item
                                char message_to_send2[255] = "";
                                sprintf(message_to_send2, "The item %s was sold for %d to the user: %s", app->items[item_index].name, amountNumber, loggedUser->username);

                                User* current_user = app->users;
                                int total_users = 0;
                                while(total_users < app->config->max_users_allowed){
                                    if(current_user->pid != -1 && current_user->pid != pid_response){
                                        send_message_frontend(message_to_send2, current_user->pid);
                                    }
                                    current_user++;
                                    total_users++;
                                }

                            }else{
                                // notify the frontend
                                char message_to_send[255] = "";
                                sprintf(message_to_send, "You have placed a bid of %d in the item %s", amountNumber, app->items[item_index].name);

                                // notify every frontend that the item got a new bid
                                char message_to_send2[255] = "";
                                sprintf(message_to_send2, "The item %s got a new bid of %d from the user: %s", app->items[item_index].name, amountNumber, loggedUser->username);

                                User* current_user = app->users;
                                int total_users = 0;
                                while(total_users < app->config->max_users_allowed){
                                    if(current_user->pid != -1 && current_user->pid != 0 && current_user->pid != pid_response){
                                        send_message_frontend(message_to_send2, current_user->pid);
                                    }
                                    current_user++;
                                    total_users++;
                                }

                            }
                        }else{
                            // the user does not have enough money
                            char message_to_send[255] = "";
                            sprintf(message_to_send, "You do not have enough money to buy this item or the bid is low");
                            send_message_frontend(message_to_send, pid_response);
                        }
                    }
                    break;
                }
                item_index++;
            }

            if(foundItem == false){
                // the item was not found
                char message_to_send[255] = "";
                sprintf(message_to_send, "The item was not found");
                send_message_frontend(message_to_send, pid_response);
            }

        }else{
            // notify the frontend
            send_message_frontend("The amount is not a number", pid_response);
        }

        reset_heartbit_counter(app, pid_response);

    }
}

// ======== ONLY BACKEND COMMANDS =========

// list promoters
void exec_command_prom(struct Backend *app)
{
    printf("     > Executing the list promoters command\n");

    Promotor *currentPromotor = app->promotors;
    int current = 0;
    while (current < app->config->max_promotors_allowed)
    {
        if (currentPromotor->valid == 1)
        {
            printf("      > Promotor %d: %s \n", currentPromotor->pid, currentPromotor->name);
        }
        currentPromotor++;
        current++;
    }
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

void exec_command_kick(struct Backend *app, struct string_list *arguments)
{
    if (arguments != NULL)
    {
        printf("     > Executing the kick user command, kicking the user: %s\n", arguments->string);

        User user = get_logged_in_user(app, -1, arguments->string);
        for (int i = 0; i < app->config->max_users_allowed; i++)
        {
            if(strcmp(user.username, arguments->string) != 0){
                printf("     > Failed to execute the kick command, user %s not found.\n", arguments->string);
                break;
            }

            if (app->frontendPids[i] == user.pid)
            {
                app->frontendPids[i] == 0;
                kill(user.pid, SIGINT);
                user.pid = -1;
            }
        }
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
void exec_command_list(struct Backend *app,int pid_response)
{
    printf("     > Executing the list all items command\n");

    int item_index = 0;

    while(item_index < app->config->max_auctions_active){
        if(app->items[item_index].active == 1){
            char message_to_send[255] = "";
            sprintf(message_to_send, "Item %d: %s, BN: %d CV: %d CT: %s CB: %s", app->items[item_index].unique_id, app->items[item_index].name, app->items[item_index].buy_now_value, app->items[item_index].current_value, app->items[item_index].category, app->items[item_index].bidder_name);
            if(pid_response != -1) {
                send_message_frontend(message_to_send, pid_response);
            }else{
                printf("Item %d: %s, BN: %d CV: %d CT: %s CB: %s\n", app->items[item_index].unique_id, app->items[item_index].name, app->items[item_index].buy_now_value, app->items[item_index].current_value, app->items[item_index].category, app->items[item_index].bidder_name);
            }
        }
        item_index++;
    }

    reset_heartbit_counter(app, pid_response);

}
