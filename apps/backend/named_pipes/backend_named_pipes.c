#include "backend_named_pipes.h"

void handle_connections_backend_read_frontend(char *CLIENT_PID){
    char arr1[80];
    int fd;
    while (1) {
        char* CLIENT_CONNECTION_TARGET = CONNECTIONS_TARGET;
        strcat(CLIENT_CONNECTION_TARGET,"_");
        strcat(CLIENT_CONNECTION_TARGET,CLIENT_PID);
        strcat(CLIENT_CONNECTION_TARGET,"_read");

        fd = open(CLIENT_CONNECTION_TARGET, O_RDONLY);
        read(fd, arr1, sizeof(arr1));

        printf("> New command received from the frontend [%s]: %s\n",CLIENT_PID, arr1);
        close(fd);

        //TODO: handle the command received!

    }
}

void handle_connections_backend_write_frontend(char *CLIENT_PID, char* message, int message_length){
    int fd1;
    char* CLIENT_CONNECTION_TARGET = CONNECTIONS_TARGET;
    strcat(CLIENT_CONNECTION_TARGET,"_");
    strcat(CLIENT_CONNECTION_TARGET,CLIENT_PID);
    strcat(CLIENT_CONNECTION_TARGET,"_write");
    mkfifo(CLIENT_CONNECTION_TARGET, 0666);

    printf("> Sent to frontend the following message: %s  \n",message);
    // Now open in write mode and write
    fd1 = open(CLIENT_CONNECTION_TARGET,O_WRONLY);
    write(fd1, message, strlen(message_length)+1);
    close(fd1);
}

void handle_connections_received_backend(Backend *backend){
    char arr1[80];
    int fd;
    while (1) {
        // Open FIFO for Read only
        fd = open(CONNECTIONS_TARGET, O_RDONLY);

        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        printf("New client connected: %s\n", arr1);
        close(fd);

        // TODO: associate the pid to list of users connected for example or somewhere
        // pid = arr1

        // TODO: associate the read thread to the connection if necessary to close in the future
        pthread_t tid_read;

        //since we have the connection registered now we will create a thread to handle the read commands from the client
        pthread_create(&tid_read, NULL, handle_connections_backend_read_frontend, NULL);
    }
}

void backend_initialize_pipes(Backend *backend){

    //create the connections pipe
    mkfifo(CONNECTIONS_TARGET, 0666);
    pthread_t tid_connections;

    //initialize a thread to receive connections
    pthread_create(&tid_connections, NULL, handle_connections_received_backend, backend);
}
