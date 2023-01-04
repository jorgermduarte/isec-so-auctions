#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/select.h>
#include "commands/initializer.h"
#include "notifier.h"
#include "../../shared/helpers/helpers.h"

Backend *app;

void unlock(int s, siginfo_t *info, void *c) {}

void close_platform(int s, siginfo_t *info, void *c)
{
    printf("\n[BACKEND] Closing platform...\n");

    // send signal to close promotors
    printf("[BACKEND] Order promoters to terminate...\n");
    for (int i = 0; i < app->config->max_promotors_allowed; i++)
    {
        if (app->promotors[i].valid == 1)
        {
            kill(app->promotors[i].pid, SIGUSR1);
        }
    }

    // send signal to close all clients
    printf("[BACKEND] Order clients to terminate...\n");
    for (int i = 0; i < app->config->max_users_allowed; i++)
    {
        if (app->frontendPids[i] != 0)
        {
            kill(app->frontendPids[i], SIGINT);
        }
    }

    sleep(2);

    // close backend threads
    printf("[BACKEND] Waiting for threads to finish...\n");
    app->threads.running = 0;

    pthread_kill(app->threads.pthread_frontend_requests, SIGUSR1);
    pthread_join(app->threads.pthread_frontend_requests, NULL);

    pthread_kill(app->threads.pthread_backend_commands, SIGUSR1);
    pthread_join(app->threads.pthread_backend_commands, NULL);

    sleep(2);

    // unlink backend fifo
    printf("[BACKEND] Cleaning backend fifo...\n");
    close(app->pipeBackend.fd);
    unlink(BACKEND_FIFO_NAME);

    exit(1);
}

void handle_system_signals()
{
    // signal SIGINT handler to close platform
    struct sigaction sa;
    sa.sa_sigaction = close_platform;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    // signal SIGINT handler to unlock threads
    struct sigaction act;
    act.sa_sigaction = unlock;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char *argv[])
{
    int nfd;
    fd_set read_fds;
    struct timeval tv;

    // handle system signals
    handle_system_signals();

    // check if this is the only backend execution
    check_backend_duplicate_execution();

    // create the backend structure that will have all data
    app = bootstrap();

    for (int i = 0; i < app->config->max_promotors_allowed; i++)
    {
        if (app->promotors[i].valid == 1)
        {
            pipe(app->promotors[i].fd);
            int fork_id = fork();
            if (fork_id == 0)
            {
                // dealing with the child process and file descriptors
                close(1);
                dup(app->promotors[i].fd[1]);
                close(app->promotors[i].fd[1]);
                close(app->promotors[i].fd[0]);

                // executing the promoter preventing the overflow of process execution with sleep
                sleep(2);
                execl(app->promotors[i].name, app->promotors[i].name, (char *)NULL);

                exit(0);
            }
            else if (fork_id > 0)
            {
                // main process
                // define or promoter pid
                app->promotors[i].pid = fork_id;
                close(app->promotors[i].fd[1]);
            }
        }
    }

    // receive messages from promoters with selects
    do
    {
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(&read_fds);

        int biggest_fd = get_max_promoter_fd(app);

        for (int i = 0; i < app->config->max_promotors_allowed; i++)
        {
            if (app->promotors[i].valid == 1)
                FD_SET(app->promotors[i].fd[0], &read_fds);
        }

        nfd = select(biggest_fd, &read_fds, NULL, NULL, &tv);
        if (nfd == -1)
        {
            perror("[ERROR] SELECT");
            kill(getpid(), SIGINT);
        }

        for (int i = 0; i < app->config->max_promotors_allowed; i++)
        {
            if (app->promotors[i].valid == 1)
            {
                if (FD_ISSET(app->promotors[i].fd[0], &read_fds))
                {
                    char buffer[260];
                    int size = read(app->promotors[i].fd[0], buffer, 256);
                    if (size > 0)
                    {
                        struct Promotions *new_promotion = malloc(sizeof(struct Promotions));

                        char* token = strtok(buffer, " ");
                        char* category = NULL;
                        char* amount_string = NULL;
                        char* time_string = NULL;

                        if (token != NULL) {
                            category = token;
                            token = strtok(NULL, " ");
                            if (token != NULL) {
                                amount_string = token;
                                token = strtok(NULL, " ");
                                if (token != NULL) {
                                    time_string = token;
                                }
                            }
                        }

                        if(category != NULL || amount_string != NULL || time_string != NULL) {

                            int id = 0;
                            if(app->promotions != NULL){
                                id = getTailPromotions(app->promotions)->id + 1;
                            }

                            //iterate time_string if we find a " " set as \0
                            for(int x = 0; x < strlen(time_string); x++){
                                if(time_string[x] == ' ' || time_string[x] == '\n' || time_string[x] == '\r'){
                                    time_string[x] = '\0';
                                }
                            }

                            for(int x = 0; x < strlen(amount_string); x++){
                                if(amount_string[x] == ' ' || amount_string[x] == '\n' || amount_string[x] == '\r'){
                                    amount_string[x] = '\0';
                                }
                            }

                            for(int x = 0; x < strlen(category); x++){
                                if(category[x] == ' ' || category[x] == '\n' || category[x] == '\r'){
                                    category[x] = '\0';
                                }
                            }



                            new_promotion->id = id;
                            new_promotion->valid = 1;
                            new_promotion->value = atoi(amount_string);
                            new_promotion->time = atoi(time_string);

                            strcpy(new_promotion->category, category);

                            struct Promotions *existentPromotion = getPromotionByCategory(&app->promotions, category);

                            if(existentPromotion != NULL && existentPromotion->valid == 1){
                                rbash();
                                printf("[ Promoter %s with pid %d ] New promotion for %s category is duplicated. Skip this promotion.\n\n", app->promotors[i].name, app->promotors[i].pid, category);
                                creset();
                                continue;
                            }else if(existentPromotion != NULL && existentPromotion->valid == 0){
                                rbash();
                                printf("[ Promoter %s with pid %d ] New promotion for %s category is duplicated. But the previous promotion is invalid. Update this promotion.\n", app->promotors[i].name, app->promotors[i].pid, category);
                                creset();
                                updatePromotion(&app->promotions, existentPromotion->id, new_promotion);
                            }else{
                                rbash();
                                printf("[ Promoter %s with pid %d ] New promotion for %s category with value %s and time %s.\n", app->promotors[i].name, app->promotors[i].pid, category, amount_string, time_string);
                                creset();
                                addPromotion(&app->promotions, new_promotion);
                            }

                            if(strlen(category) > 0 && strlen(amount_string) > 0 && strlen(time_string) > 0){

                                int current_user_index_f = 0;
                                while( current_user_index_f < app->config->max_users_allowed){
                                    if(app->users[current_user_index_f].pid > 0){
                                        //printf(" > sending message to frontend application with pid %d.\n", app->users[current_user_index_f].pid);

                                        char message_to_send_2[255] = "[PROM] ";
                                        //strcat(message_to_send, category);
                                        strcat(message_to_send_2, " with ");
                                        strcat(message_to_send_2, amount_string);
                                        strcat(message_to_send_2, " percent off for ");
                                        strcat(message_to_send_2, time_string);
                                        strcat(message_to_send_2, " seconds.");
                                        // printf(" preparing to send the message: %s - to all frontend applications.\n", message_to_send);
                                        // TODO: proms disappear after sending the message to the frontend applications
                                        //send_message_frontend(message_to_send_2, app->users[current_user_index_f].pid);
                                    }

                                    current_user_index_f++;
                                }

                            }

                        }
                    }
                    else
                    {
                        // printf(" > Reached the end of the pipe for promoter %s - p%d \n", app->promotors[i].name, app->promotors[i].pid);
                        app->promotors[i].valid = 0;
                        kill(app->promotors[i].pid, SIGINT);
                    }
                }
            }
        }
        sleep(1);
    } while (1);
}
