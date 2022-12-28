#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include "../../../shared/models/communication.h"
#include "../commands/initializer.h"
#include "../../../shared/helpers/pipes.h"
#include "notifier.h"

typedef struct Threads
{
    pthread_t pthread_frontend_responses;
    int lock;
} Threads;

typedef struct Frontend
{
    pid_t pid;
    int fifo_fd;
    char * fifo_name;
    Threads frontend_responses;

} Frontend;

void initialize_frontend(char *username, char *password);
int fifo_exists(char* filename);