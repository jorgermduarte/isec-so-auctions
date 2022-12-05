#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "user.h"
#include "promotor.h"
#include "auction.h"
#include "../../../shared/config/config.h"

#define BNAMEFIFO "FIFOBACKEND1"
#define FNAMEFIFO "FIFOFRONTEND"

typedef struct Threads
{
    pthread_t pthread_backend_commands;
    pthread_t pthread_frontend_requests;
} Threads;

typedef struct Backend
{
    Config *config;
    Auction *auctions;
    Item *items;
    Promotor *promotors;
    User *users;

    Threads threads;

    int fifo_b;
} Backend;

Backend *bootstrap();

void load_items_from_file(char* filename, Item *items);
User *load_users_from_file(char *filename);
Promotor *load_promotors_from_file(char *filename);

void *command_thread_handler(void *pdata);
void *frontend_communication_receiver_handler(void *pdata);

int copen_fifo_backend();
