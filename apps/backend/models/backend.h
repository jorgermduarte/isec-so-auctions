#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "item.h"
#include "user.h"
#include "promotor.h"
#include "auction.h"
#include "../../../shared/config/config.h"

typedef struct Threads
{
    pthread_t pthread_backend_commands;
} Threads;

typedef struct Backend
{
    Config *config;
    Auction *auctions;
    Promotor *promotors;
    User *users;

    Threads threads;
} Backend;

Backend *bootstrap();

Item *load_items_from_file(char *filename);
User *load_users_from_file(char *filename);
Promotor *load_promotors_from_file(char *filename);

void *command_thread_handler(void *pdata);