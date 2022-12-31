#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include "user.h"
#include "promotor.h"
#include "auction.h"
#include "../config/config.h"
#include "../../../shared/helpers/pipes.h"

typedef struct Threads
{
    pthread_t pthread_backend_commands;
    pthread_t pthread_frontend_requests;
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
    Auction *auctions;
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

void frontend_communication_fifo_initializer();
void check_backend_duplicate_execution();

int get_max_promoter_fd(Promotor *promoters, int size);
void read_promoter_message(Promotor promoter, fd_set read_fds);
int assign_unknown_client(pid_t pid, int *arr, int length);
