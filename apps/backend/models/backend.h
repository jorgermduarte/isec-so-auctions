#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include "user.h"
#include "promotor.h"
#include "../config/config.h"
#include "../../../shared/helpers/pipes.h"

typedef struct Threads
{
    pthread_t pthread_backend_commands;
    pthread_t pthread_frontend_requests;
    pthread_t pthread_frontend_heartbit;
    pthread_t pthread_auctions_duration;
    int running;
} Threads;

typedef struct Pipes
{
    int fd;
    int pid;
    int valid;
} Pipes;

typedef struct Backend
{
    Config *config;    
    Item *items;
    Promotor *promotors;
    User *users;

    Threads threads;
    Pipes pipeBackend;
    int frontendPids[20];

    int fifo_b;
} Backend;

Backend* bootstrap();

void load_items_from_file(char* filename, Backend* app);
void load_users_from_file(char *filename, Backend* app);
void *load_promoters_from_file(char *filename, Promotor *promoters);

void *command_thread_handler(void *pdata);
void *frontend_communication_receiver_handler(void *pdata);
void *frontend_heartbit_handler(void *pdata);
void *auctions_duration_handler(void *pdata);

void frontend_communication_fifo_initializer();
void check_backend_duplicate_execution();

int get_max_promoter_fd(struct Backend *app);
int assign_or_return_client_index(pid_t pid, int *arr, int length);

int reset_heartbit_counter(Backend* app, pid_t pid);

User get_logged_in_user(Backend* app, pid_t pid, char *seller);
User* find_user_by_pid(Backend *app, pid_t pid);