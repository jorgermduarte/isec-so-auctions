#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../shared/helpers/pipes.h"
#include "../../shared/models/communication.h"

void send_message_backend(char *message) {
    int fd;

    pid_t pid = getpid();
    Request request;
    strcpy(request.arguments, message);

    Response response = { .result = ""};
    printf(" > [%d] Sending to backend the following message: %s  \n",pid,message);

    Message msg;

    msg.pid = pid;
    msg.request = request;
    msg.response = response;

    // Now open in write mode and write
    fd = open(BACKEND_FIFO_NAME,O_WRONLY);
    write(fd, &msg, sizeof(msg));
    close(fd);
}