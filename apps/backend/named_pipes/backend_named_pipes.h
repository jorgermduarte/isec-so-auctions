#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "../models/backend.h"

char * CONNECTIONS_TARGET = "/tmp/connections";

void backend_initialize_pipes(Backend *backend);

void handle_connections_backend_write_frontend(char *CLIENT_PID, char* message, int message_length);
