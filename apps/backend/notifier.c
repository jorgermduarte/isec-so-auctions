#include <string.h>
#include "../../shared/helpers/pipes.h"
#include "../../shared/models/communication.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

void send_message_frontend(char *message, int pid){
    int fd;
    Response response;
    Request request;

    strcpy(response.result, message);

    Message msg;
    msg.pid = getpid();
    msg.request = request;
    msg.response = response;

    char pipe_name[100] = "";
    char pid_string[10];
    // integer to string
    sprintf(pid_string, "%d", pid);
    strcat(pipe_name, FRONTEND_FIFO_NAME_BASE);
    strcat(pipe_name, pid_string);
    printf("        > Sending message to the following pipe: %s\n",pipe_name);

    //create pipe if it doesn't exist
    if (mkfifo(pipe_name, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            //printf("    > [ERR] FRONTEND FIFO ALREADY EXISTS\n");
        }
    }
    fd = open(pipe_name,O_RDWR);
    if (fd == -1)
    {
        printf("    > [ERR] OPENING FRONTEND FIFO\n");
    }else{
        write(fd, &msg, sizeof(msg));
    }
    close(fd);
}