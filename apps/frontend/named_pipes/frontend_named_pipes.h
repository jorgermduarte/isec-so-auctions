#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

char* CONNECTIONS_TARGET = "/tmp/connections";

void frontend_initialize_connection();

void handle_connections_frontend_write_backend(char *CLIENT_PID, char* message, int message_length);

void handle_connections_frontend_read_backend();