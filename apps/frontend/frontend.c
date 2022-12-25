#include "models/frontend.h"
#include "models/authentication.h"

void backend_communication_fifo_initializer(char* pipe_name)
{
    //create the named pipe for the frontend to receive responses from the backend
    if (mkfifo(pipe_name, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            //perror("\n[ERR] FRONTEND FIFO ALREADY EXISTS");
        }else{
            perror("\n[ERR] ERROR WHILE CREATING FIFO");
            exit(1);
        }
    }
}

void* backend_communication_receiver_handler(void* data)
{
    char* pipe_name = (char*)data;
    Message msg;
    printf(" > [INFO] FRONTEND COMMUNICATION RECEIVER HANDLER STARTED FOR PIPE: %s\n",pipe_name);

    while(1){
        int fd_bfifo = open(pipe_name,  O_RDONLY | O_NONBLOCK);
        if(fd_bfifo == -1){
            //printf(" > Something went wrong receiving the message from a backend application \n");
        }else{
            int read_size = sizeof(msg) + 1;
            int size = read(fd_bfifo, &msg, read_size);
            if(size > 0) {
                printf(" > [RECEIVED RESPONSE FROM BACKEND]: %s\n", msg.response.result);

                //verify if the message is a response to a login request
                if(strcmp(msg.response.result, "LOGIN_SUCCESS") == 0){
                    //set the user as logged in
                    auth.is_logged_in = 1;
                    printf(" > [INFO] YOU HAVE LOGGED IN SUCCESSFULLY\n");
                }
            }
            close(fd_bfifo);
        }
    }
}

void initialize_frontend(char* username, char* password){

    //initialize the commands receiver fifo from the backend application
    char pipe_name_result[100] = "";

    pid_t pid = getpid();
    char pid_string[10];
    sprintf(pid_string, "%d", pid); // integer to string
    char* pipe_name = FRONTEND_FIFO_NAME_BASE;
    strcat(pipe_name_result,FRONTEND_FIFO_NAME_BASE);
    strcat(pipe_name_result,pid_string);
    printf(" > [INFO] FRONTEND PIPE NAME DEFINED: %s\n", pipe_name_result);

    backend_communication_fifo_initializer(pipe_name);

    //create a thread to receive the responses from the backend
    pthread_t pthread_frontend_responses;
    pthread_create(&pthread_frontend_responses, NULL, backend_communication_receiver_handler, (void*)pipe_name_result);

    //define user frontend information
    auth.is_logged_in = 0;
    strcpy(auth.username, username);
    strcpy(auth.password, password);
    auth.p_id = pid;

    // prepare the login message to authenticate the user in the backend
    char command_login[255] = "login ";
    strcat(command_login, username);
    strcat(command_login, " ");
    strcat(command_login, password);
    sleep(1);

    //send backend the login message
    send_message_backend(command_login);


    // verify user authentication
    int timeout = 5;
    while(auth.is_logged_in == 0){
        sleep(1);
        if(auth.is_logged_in == 0){
            printf(" > Verifying user authentication...\n");
        }
        timeout--;
        if(timeout <= 0){
            printf(" > Failed to authenticate the user, closing the application ...\n");
            exit(1);
        }
    }

    if(auth.is_logged_in == 1){
        command_handler_start();
    }
}