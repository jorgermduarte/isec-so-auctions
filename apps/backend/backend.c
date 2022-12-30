#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "./commands/initializer.h"
#include "../../shared/helpers/helpers.h"
#include "../../shared/helpers/constants.h"
#include "../../shared/models/communication.h"

Backend *bootstrap()
{
    // Create main structures for the backend to work
    Backend *app = malloc(sizeof(Backend));

    // define config values
    Config *config = get_env_variables();
    app->config = config;

    int file_item_size = get_file_size(app->config->f_items);

    // Read data from files
    Item items[file_item_size];
    load_items_from_file(app->config->f_items, &items[0]);
    app->items = items;

    // load users from file
    load_users_from_file(app->config->f_users, app);

    Promotor promoters[app->config->max_promotors_allowed];
    load_promoters_from_file(app->config->f_promotors, &promoters[0]);
    app->promotors = promoters;

    memset(app->frontendPids, 0, sizeof(int));    

    // initialize the commands receiver fifo from the frontend applications
    frontend_communication_fifo_initializer();

    // Instantiate threads
    app->threads.running = 1;
    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, app);
    pthread_create(&app->threads.pthread_frontend_requests, NULL, frontend_communication_receiver_handler, app);

    return app;
}

void *command_thread_handler(void *pdata)
{
    // TODO: right now we are not doing anything with the Backend app structure
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

void load_users_from_file(char* filename, Backend* app) {
    // Open the file for reading
    FILE* file = fopen(filename, "r");

    User users[app->config->max_users_allowed];

    // Check if the file was successfully opened
    if (file == NULL) {
        fprintf(stderr, "Error: failed to open file '%s'\n", filename);
        return;
    }

    printf("    >  Loading users from file: %s\n", filename);

    // Read the file line by line
    char line[256];
    int num_users = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Parse the line to get the username, password, and budget
        char username[50];
        char password[50];
        int budget;
        int num_scanned = sscanf(line, "%s %s %d", username, password, &budget);


        // Check if the line was successfully parsed
        if (num_scanned == 3) {
            User newUser;
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
        app->pipeBackend.fd = open(BACKEND_FIFO_NAME, O_RDONLY | O_NONBLOCK);
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
                printf(" > [FRONTEND %d] [REQUESTED] %s", msg.pid, msg.request.arguments);

                // handle the commands received and sed the response
                struct string_list *arguments = get_command_arguments(msg.request.arguments);
                int close_app = command_try_execution(arguments->string, arguments->next, msg.pid, app);
                assign_unknown_client(msg.pid, &app->frontendPids[0], app->config->max_users_allowed);                

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
        // the %20s is to avoid buffer overflows
        sscanf(line, "%20s %20s %20s %d %d %d %20s %20s", it.identifier, it.name, it.category, &it.duration, &it.current_value, &it.buy_now_value, it.seller_name, it.bidder_name);
        items[i++] = it;
    }
    fclose(fp);
    if (line)
        free(line);
}

void *load_promoters_from_file(char *filename, Promotor *promoters)
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
        Promotor it;
        // the %20s is to avoid buffer overflow
        memset(it.name, 0, sizeof(it.name));

        sscanf(line, "%20s", it.name);

        it.valid = 1;
        promoters[i++] = it;
    }

    fclose(fp);
    if (line)
        free(line);
}

int get_max_promoter_fd(Promotor *promoters, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        if(promoters[i].valid != 1){
            return promoters[--i].fd[0] + 1;
        }
    }
    
}

void read_promoter_message(Promotor promoter, fd_set read_fds)
{
    char buffer[20] = "\0";
    if (promoter.valid == 1)
    {
        if (FD_ISSET(promoter.fd[0], &read_fds))
        {   
            int size = read(promoter.fd[0], buffer, sizeof(buffer));
            if (size != 0)
            {
                rbash();
                printf("\n[Promoter %s - p%d] sent the following message : %s\033[0m", promoter.name, promoter.pid, buffer);
                creset();
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
}

int assign_unknown_client(pid_t pid, int *arr, int length){

    int arrLen = length;
    int isElementPresent = 0;
     
    for (int i = 0; i < arrLen; i++) {
        if (arr[i] == pid) {
            isElementPresent = 1;
            break;
        }
    }

    if(isElementPresent == 0){
        for (int i = 0; i < arrLen; i++) {
            if (arr[i] == 0){
                arr[i] = pid;
                break;
            }
        }
    }

    return isElementPresent;
}