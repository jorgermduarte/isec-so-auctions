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
                    char buffer[25];
                    int size = read(app->promotors[i].fd[0], buffer, 256);
                    if (size > 0)
                    {
                        struct Promotions *new_promotion = malloc(sizeof(struct Promotions));
                        char message_to_send[256] = "";

                        char* token = strtok(buffer, " ");
                        char *category = token;
                        token = strtok(NULL, " ");
                        int amount = atoi(token);
                        token = strtok(NULL, " ");
                        int time = atoi(token);

                        int id = 0;
                        if(app->promotions != NULL){
                            id = getTailPromotions(app->promotions)->id + 1;
                        }
                        new_promotion->id = id;
                        new_promotion->valid = 1;
                        new_promotion->value = amount;
                        new_promotion->time = time;
                        strcpy(new_promotion->category, category);

                        struct Promotions *existentPromotion = getPromotionByCategory(&app->promotions, category);
                        if(existentPromotion != NULL && existentPromotion->valid == 1){
                            printf("\n[ Promoter %s with pid %d ] New promotion for %s category is duplicated. Skip this promotion.\n\n", app->promotors[i].name, app->promotors[i].pid, category, amount, time);
                            continue;
                        }else if(existentPromotion != NULL && existentPromotion->valid == 0){
                            updatePromotion(&app->promotions, existentPromotion->id, new_promotion);
                        }else{
                            addPromotion(&app->promotions, new_promotion);    
                        }

                        rbash();
                        printf("\n[ Promoter %s with pid %d ] New promotion for %s category started with %d%% off during %d seconds.\n\n", app->promotors[i].name, app->promotors[i].pid, category, amount, time);
                        creset();

                        char * string_amount = malloc(sizeof(char)*9);
                        char * string_time = malloc(sizeof(char)*9);
                        sprintf(string_amount, "%d", amount);
                        sprintf(string_time, "%d", time);

                        strcat(message_to_send, "New promotion for ");
                        strcat(message_to_send, category);
                        strcat(message_to_send, " category started with ");
                        strcat(message_to_send, string_amount);
                        strcat(message_to_send, " percent off during ");
                        strcat(message_to_send, string_time);
                        strcat(message_to_send, " seconds.");

                        for(int i = 0; i < app->config->max_users_allowed; i++){
                            if(app->frontendPids[i] != 0){
                                send_message_frontend(message_to_send, app->frontendPids[i]);
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
