#include <signal.h>
#include "models/frontend.h"
#include "models/authentication.h"

Frontend *client;

void unlock(int s, siginfo_t *info, void *c) {}

void close_frontend(int s, siginfo_t *info, void *c)
{
    printf("\n[INFO] Backend closing.. this client will be closed in a moment\n");

    
    printf("[INFO] Cleaning frontend fifo\n");
    
    close(client->fifo_fd);
    unlink(client->fifo_name);

    sleep(2);

    printf("[INFO] Waiting for threads to finish...\n");
    client->frontend_responses.lock = 0;
    pthread_kill(client->frontend_responses.pthread_frontend_responses, SIGUSR1);
    pthread_join(client->frontend_responses.pthread_frontend_responses, NULL);

    exit(1);
}


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
    Frontend* client = (Frontend*)data;
    Message msg;
    printf(" > [INFO] FRONTEND COMMUNICATION RECEIVER HANDLER STARTED FOR PIPE: %s\n",client->fifo_name);

    while(client->frontend_responses.lock){
        client->fifo_fd = open(client->fifo_name,  O_RDWR);
        if(client->fifo_fd == -1){
            //printf(" > [WRN] Something went wrong receiving the message from a backend application \n");
        }else{
            int read_size = sizeof(msg) + 1;
            int size = read(client->fifo_fd, &msg, read_size);
            if(size > 0) {
                printf(" > [RECEIVED RESPONSE FROM BACKEND]: %s\n", msg.response.result);

                //verify if the message is a response to a login request
                if(strcmp(msg.response.result, "LOGIN_SUCCESS") == 0){
                    //set the user as logged in
                    auth.is_logged_in = 1;
                    printf(" > [INFO] YOU HAVE LOGGED IN SUCCESSFULLY\n");
                }
            }
        }
        close(client->fifo_fd);
    }
}

void initialize_frontend(char* username, char* password){

    // signal SIGINT handler to close platform
    struct sigaction sa;
    sa.sa_sigaction = close_frontend;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    // signal SIGINT handler to unlock threads
    struct sigaction act;
    act.sa_sigaction = unlock;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    // allocate memory for frontend struct
    client = malloc(sizeof(Frontend));

    //initialize the commands receiver fifo from the backend application
    char pipe_name_result[100] = "";

    client->pid = getpid();

    char pid_string[10];
    sprintf(pid_string, "%d", client->pid); // integer to string
    char* pipe_name = FRONTEND_FIFO_NAME_BASE;
    strcat(pipe_name_result,FRONTEND_FIFO_NAME_BASE);
    strcat(pipe_name_result,pid_string);
    printf(" > [INFO] FRONTEND PIPE NAME DEFINED: %s\n", pipe_name_result);

    client->fifo_name = pipe_name_result;
    client->frontend_responses.lock = 1;

    backend_communication_fifo_initializer(pipe_name_result);

    //create a thread to receive the responses from the backend
    pthread_create(&client->frontend_responses.pthread_frontend_responses, NULL, backend_communication_receiver_handler, client);

    //define user frontend information
    auth.is_logged_in = 0;
    strcpy(auth.username, username);
    strcpy(auth.password, password);
    auth.p_id = client->pid;

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

int fifo_exists(char* filename){
    struct stat stat_p;

    stat (filename, &stat_p);

    return S_ISFIFO(stat_p.st_mode);
}