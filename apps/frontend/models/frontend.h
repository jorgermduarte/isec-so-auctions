#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include "../../../shared/models/communication.h"
#include "../commands/initializer.h"
#include "../../../shared/helpers/pipes.h"
#include "notifier.h"

void initialize_frontend(char* username, char* password);