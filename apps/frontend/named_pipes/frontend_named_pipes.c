#include "frontend_named_pipes.h"

void frontend_initialize_connection(){
    int fd1;
    // in this case the frontend will write to the read since the backend is reading from here
    strcat(CONNECTIONS_TARGET,"_read");
    mkfifo(CONNECTIONS_TARGET, 0666);

    int pid = getpid();
    char spid[10];
    sprintf(spid, "%d", pid);

    printf("> Signaling the backend from our frontend connection, PID -> %s  \n",spid);
    fd1 = open(CONNECTIONS_TARGET,O_WRONLY);
    write(fd1, spid, strlen(spid)+1);
    close(fd1);
}

void handle_connections_frontend_read_backend(){
    char arr1[80];
    int fd;

    int pid = getpid();
    char CLIENT_PID[10];
    sprintf(CLIENT_PID, "%d", pid);

    char* CLIENT_CONNECTION_TARGET = CONNECTIONS_TARGET;
    strcat(CLIENT_CONNECTION_TARGET,"_");
    strcat(CLIENT_CONNECTION_TARGET,CLIENT_PID);
    // in this case the frontend will read the backend outputs
    strcat(CLIENT_CONNECTION_TARGET,"_write");
    mkfifo(CLIENT_CONNECTION_TARGET, 0666);
    while (1) {
        fd = open(CLIENT_CONNECTION_TARGET, O_RDONLY);
        read(fd, arr1, sizeof(arr1));

        printf("> New response received from the backend: %s\n", arr1);
        close(fd);
        //TODO: handle the response received!

    }
}

void handle_connections_frontend_write_backend(char *CLIENT_PID, char* message, int message_length){
    int fd1;
    char* CLIENT_CONNECTION_TARGET = CONNECTIONS_TARGET;
    strcat(CLIENT_CONNECTION_TARGET,"_");
    strcat(CLIENT_CONNECTION_TARGET,CLIENT_PID);
    // in this case the frontend will write to the read since the backend is reading from here
    strcat(CLIENT_CONNECTION_TARGET,"_read");
    mkfifo(CLIENT_CONNECTION_TARGET, 0666);

    printf("> Sent from frontend to backend the following message: %s  \n",message);
    fd1 = open(CLIENT_CONNECTION_TARGET,O_WRONLY);
    write(fd1, message, strlen(message_length)+1);
    close(fd1);
}