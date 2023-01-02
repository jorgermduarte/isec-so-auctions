#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/select.h>
#include "commands/initializer.h"
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
        if(app->frontendPids[i] != 0){
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
            // TODO: detect bug here only starts one of the promoters           
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
    // TODO: bug detected after some promoters messages we receive spam! (not sure if it is the last line)
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
        if(nfd == -1){
            perror("[ERROR] SELECT");
            kill(getpid(), SIGINT);
        }

        for (int i = 0; i < app->config->max_promotors_allowed; i++)
        {
            if(app->promotors[i].valid == 1){
                if (FD_ISSET(app->promotors[i].fd[0], &read_fds))
                {
                    char buffer[1024];
                    read(app->promotors[i].fd[0], buffer, 1024);
                    rbash();
                    printf("\n[Promoter %s - p%d] sent the following message : %s\033[0m", app->promotors[i].name, app->promotors[i].pid, buffer);
                    creset();
                }
            }
        }
        sleep(0.1);
    } while (1);
}
