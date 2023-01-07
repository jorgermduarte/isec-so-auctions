#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "notifier.h"
#include "./commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "../../shared/helpers/constants.h"
#include "../../shared/models/communication.h"
#include "../../users_lib.h"

Backend *bootstrap()
{
    // Create main structures for the backend to work
    Backend *app = malloc(sizeof(Backend));

    // define config values
    Config *config = get_env_variables();
    app->config = config;
    app->promotions = NULL;

    // define locks
    pthread_mutex_init(&app->locks.buy_lock, NULL);
    pthread_mutex_init(&app->locks.sell_lock, NULL);

    // Read data from files
    load_items_from_file(app->config->f_items, app);

    // load users from file
    load_users_from_file(app->config->f_users, app);

    // load promoters from file
    load_promoters_from_file(app->config->f_promotors, app);

    memset(app->frontendPids, 0, sizeof(int));

    // initialize the commands receiver fifo from the frontend applications
    frontend_communication_fifo_initializer();

    // Instantiate threads
    app->threads.running = 1;
    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, app);
    pthread_create(&app->threads.pthread_frontend_requests, NULL, frontend_communication_receiver_handler, app);
    pthread_create(&app->threads.pthread_frontend_heartbit, NULL, frontend_heartbit_handler, app);
    pthread_create(&app->threads.pthread_promotions_duration, NULL, promotions_duration_handler, app);
    pthread_create(&app->threads.pthread_auctions_duration, NULL, auctions_duration_handler, app);

    return app;
}

void *promotions_duration_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;
    while(app->threads.running){

        struct Promotions *current = app->promotions;

        // do verification for each promotion
        while(current != NULL){
            if(current->valid == 1){
                if(current->time <= 0){

                    printf(">   Promotion %s has ended.\n", current->category);
                    current->valid = 0;

                    int current_user_index = 0;
                    while( current_user_index < app->config->max_users_allowed) {
                        if (app->users[current_user_index].pid > 0) {
                            // send notification to user
                            char messsage_to_frontend[255] = "The promotion ";
                            strcat(messsage_to_frontend, current->category);
                            strcat(messsage_to_frontend, " has ended.");
                            send_message_frontend(messsage_to_frontend, app->users[current_user_index].pid);

                        }
                        current_user_index++;
                    }
                    current = current->next;
                }else{
                    current->time -= 1;
                    current = current->next;
                }
            }else{
                //int id_delete = current->id;
                //removePromotion(&app->promotions, id_delete);
                current = current->next;
            }
        }

        //cleanup promotions
        /*struct Promotions *current2 = app->promotions;
        while(current2 != NULL){
            if(current2->valid == 0){
                //int id_delete = current2->id;
                //current2 = current2->next;
                //removePromotion(&app->promotions, id_delete);
               // printf(" >  [CLEAN] Promotion %d has been removed.\n", id_delete);

            }else{
                current2 = current2->next;
            }
        }*/

        sleep(1);
    }
}

void *auctions_duration_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;

    while (app->threads.running)
    {
        int i = 0;
        while(i < app->config->max_auctions_active){
            if (app->items[i].active == 1 & app->items[i].duration == 0)
            {
                printf(" detected an item: %s with the duration: %d\n", app->items[i].name, app->items[i].duration);
                app->items[i].active = 0;

                char message_to_send[256] = "";

                struct Promotions *active_promotion = getPromotionByCategory(&app->promotions, app->items[i].category);

                // send message to all frontend applications
                int current_user = 0;
                while ( current_user < app->config->max_users_allowed)
                {
                    if (app->users[current_user].pid != 0 && app->users[current_user].pid != -1)
                    {
                        if(strcmp(app->users[current_user].username, app->items[i].seller_name) == 0 && strcmp(app->items[i].bidder_name, "-") != 0) // is the seller of the item and have a bidder on the item
                        {
                            // update seller balance with the current value aka the sell price.
                            int userBalance = getUserBalance(app->users[current_user].username);
                            updateUserBalance(app->users[current_user].username, userBalance + app->items[i].current_value);
                            printf(">   Adding %d€ to the seller %s account.\n", app->items[i].current_value, app->users[current_user].username);

                            //notify the seller than is auction is over
                            sprintf(message_to_send,"Your auction for the item %s has ended. The winner is %s with a bid of %d ", app->items[i].name, app->items[i].bidder_name, app->items[i].current_value);
                            send_message_frontend(message_to_send,app->users[current_user].pid);
                        }
                        else if(strcmp(app->users[current_user].username, app->items[i].bidder_name) == 0){ // is the winner of the auction

                            int userBalance = getUserBalance(app->users[current_user].username);
                            double discount_percentage = 100;

                            if(active_promotion != NULL){
                                discount_percentage = (discount_percentage - (double)active_promotion->value) / 100;
                            }else{
                                discount_percentage = 1;
                            }

                            int amount_to_pay = app->items[i].current_value * discount_percentage;

                            printf(">   Paying %d€ by the bidder %s.\n", amount_to_pay, app->items[i].bidder_name);
                            updateUserBalance(app->users[current_user].username, userBalance - amount_to_pay);

                            //notify the winner than is auction is over
                            sprintf(message_to_send, "You won the auction for the item %s with a bid of %d but payed %d",app->items[i].name, app->items[i].current_value, amount_to_pay);
                            send_message_frontend(message_to_send, app->users[current_user].pid);

                        }
                        else{
                            if(strcmp(app->users[current_user].username, "") != 0 && app->users[current_user].pid != 0 && app->users[current_user].pid != -1 ){ // is a user that is not the seller or the winner
                                sprintf(message_to_send, "Auction for item %s has ended.", app->items[i].name);
                                send_message_frontend(message_to_send, app->users[current_user].pid);
                            } else {
                                printf("    > Auction for item %s has ended.\n", app->items[i].name);
                            }
                        }
                    }
                    current_user++;
                }


            }else if( app->items[i].active == 1){
                app->items[i].duration--;
            }
            i++;
        }
        sleep(1);

    }
}

void *frontend_heartbit_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;

    while (app->threads.running)
    {
        for (int i = 0; i < app->config->max_users_allowed; i++)
        {
            if (app->users[i].pid != -1)
            {
                app->users[i].heartbit--;

                if (app->users[i].heartbit == 0)
                {
                    printf(" > Frontend %d is afk. Order its closure. \n", app->users[i].pid);
                    kill(app->users[i].pid, SIGINT);
                    int pos = assign_or_return_client_index(app->users[i].pid, app->frontendPids, app->config->max_users_allowed);
                    app->users[i].pid = -1;
                    app->frontendPids[pos] = 0;
                }
            }
        }

        sleep(1);
    }
}

void *command_thread_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;

    // initialize the logic to receive command inputs
    command_handler_start(app);
}

void frontend_communication_fifo_initializer()
{
    // create the named pipe for the frontend to send requests and the backend read them
    if (mkfifo(BACKEND_FIFO_NAME, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            // perror("\n[ERR] FIFO ALREADY EXISTS");
        }
        else
        {
            perror("\n[ERR] ERROR WHILE CREATING FIFO");
            exit(1);
        }
    }
}

void load_users_from_file(char *filename, Backend *app)
{
    // Open the file for reading
    FILE *file = fopen(filename, "r");

    User users[app->config->max_users_allowed];

    // Check if the file was successfully opened
    if (file == NULL)
    {
        fprintf(stderr, "Error: failed to open file '%s'\n", filename);
        return;
    }

    loadUsersFile(app->config->f_users);

    printf("    >  Loading users from file: %s\n", filename);

    //iterate all users list and clean all of pids
    for (int i = 0; i < app->config->max_users_allowed; i++)
    {
        users[i].pid = -1;
    }

    // Read the file line by line
    char line[256];
    int num_users = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Parse the line to get the username, password, and budget
        char username[50] = "\0";
        char password[50] = "\0";
        int budget;
        int num_scanned = sscanf(line, "%s %s %d", username, password, &budget);

        // Check if the line was successfully parsed
        if (num_scanned == 3)
        {
            User newUser;
            memset(&newUser, 0, sizeof(User));

            strcpy(newUser.username, username);
            strcpy(newUser.password, password);
            newUser.budget = budget;
            printf("        > read: %s %s %d\n", username, password, budget);
            users[num_users] = newUser;
            num_users++;
        }
    }

    // define users
    app->users = users;

    // Close the file
    fclose(file);
}

void *frontend_communication_receiver_handler(void *pdata)
{
    Backend *app = (Backend *)pdata;
    Message msg;

    while (app->threads.running)
    {
        app->pipeBackend.fd = open(BACKEND_FIFO_NAME, O_RDONLY);
        app->pipeBackend.pid = getpid();
        if (app->pipeBackend.fd == -1)
        {
            if (app->threads.running > 0)
                printf(" > Something went wrong receiving the message from a frontend application \n");
        }
        else
        {
            int size = read(app->pipeBackend.fd, &msg, sizeof(msg));
            if (size > 0)
            {
                printf("\n[FRONTEND %d] [REQUESTED] %s\n", msg.pid, msg.request.arguments);

                // handle the commands received and sed the response
                struct string_list *arguments = get_command_arguments(msg.request.arguments);
                int close_app = command_try_execution(arguments->string, arguments->next, msg.pid, app);
                assign_or_return_client_index(msg.pid, &app->frontendPids[0], app->config->max_users_allowed);

                if (!close_app)
                {
                    // clean the arguments memory allocated to avoid memory leaks
                    clean_linked_list(arguments);
                }
            }
            close(app->pipeBackend.fd);
        }
    }
}

void check_backend_duplicate_execution()
{
    // create the named pipe for the frontend to send requests and the backend read them
    if (mkfifo(BACKEND_FIFO_NAME, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("[ERROR] - A BACKEND INSTANCE IS ALREADY RUNNING\n");
            printf("[ERROR] - CLOSING THIS INSTANCE.\n");
            exit(0);
        }
    }
}

void load_items_from_file(char *filename, Backend *app)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    Item *itemsDefine = malloc(sizeof(Item) * app->config->max_auctions_active);
    int i = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        Item it;
        // the %20s is to avoid buffer overflows
        sscanf(line, "%20s %20s %20s %d %d %d %20s %20s", it.identifier, it.name, it.category, &it.current_value,  &it.buy_now_value, &it.duration, it.seller_name, it.bidder_name);
        it.active = 1;
        it.unique_id = i;
        itemsDefine[i] = it;
        printf("    >  Loading item: %s for %d seconds\n", it.name, it.duration);
        i++;
    }
    fclose(fp);

    printf("        > Total items loaded: %d\n", i);

    app->items = itemsDefine;

    if (line)
        free(line);
}

void load_promoters_from_file(char *filename, Backend *app) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    Promotor *promoters = malloc(sizeof(Promotor) * app->config->max_promotors_allowed);

    //clean all promoters
    for (int i = 0; i < app->config->max_promotors_allowed; i++) {
        Promotor it;
        it.pid = -1;
        it.valid = 0;
        promoters[i] = it;
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int current = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        Promotor it;
        // the %20s is to avoid buffer overflow
        memset(it.name, 0, sizeof(it.name));

        sscanf(line, "%20s", it.name);

        strcpy(promoters[current].name, it.name);
        promoters[current].valid = 1;
        current++;
    }

    // display the promoters
    printf("    >  Loaded promoters from file: %s\n", filename);
    for (int i = 0; i < app->config->max_promotors_allowed; i++) {
        if (promoters[i].valid) {
            printf("        > promoter: %s\n", promoters[i].name);
        }
    }

    app->promotors = promoters;
    fclose(fp);
    if (line)
        free(line);
}


int get_max_promoter_fd(struct Backend* app)
{
    int max = 0;
    for (int i = 0; i < app->config->max_promotors_allowed; i++)
    {
        if (app->promotors[i].valid == 1)
        {
            max = i;
        }
    }
    // always returns plus 1 because the index starts at 0

    return app->promotors[max].fd[0] + 1;
}

int assign_or_return_client_index(pid_t pid, int *arr, int length)
{

    int arrLen = length;
    int isElementPresent = 0;

    for (int i = 0; i < arrLen; i++)
    {
        if (arr[i] == pid)
        {
            isElementPresent = i;
            break;
        }
    }

    if (isElementPresent == 0)
    {
        for (int i = 0; i < arrLen; i++)
        {
            if (arr[i] == 0)
            {
                arr[i] = pid;
                break;
            }
        }
    }

    return isElementPresent;
}

int reset_heartbit_counter(Backend *app, pid_t pid)
{
    for (int i = 0; i < app->config->max_users_allowed; i++)
    {
        if (app->users[i].pid != -1 && app->users[i].pid == pid)
        {
            app->users[i].heartbit = app->config->frontend_keep_alive_seconds;
            return 1;
        }
    }

    return 0;
}

User* find_user_by_pid(Backend *app, pid_t pid)
{
    for (int i = 0; i < app->config->max_users_allowed; i++)
    {
        if (app->users[i].pid != -1 && app->users[i].pid == pid)
        {
            return &app->users[i];
        }
    }

    return NULL;
}

User get_logged_in_user(Backend *app, pid_t pid, char *seller)
{
    for (int i = 0; i < app->config->max_users_allowed; i++)
    {
        if (pid != -1 && app->users[i].pid == pid)
        {
            return app->users[i];
        }

        if (seller != "" && strcmp(app->users[i].username, seller) == 0)
        {
            return app->users[i];
        }
    }
}

