#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

Config *get_env_variables() {
    Config *config = malloc(sizeof(Config));
    bool file_environment_variables = false;

    int aux_numeric_env[4];
    char *aux_string_env[3];
    char err_msg[256] = "\n Environment variable missing. Assuming fallback value of 10";

    char *max_promotor = getenv("MAX_PROMOTOR");
    char *max_users = getenv("MAX_USERS");
    char *max_active_auctions = getenv("MAX_ACTIVE_AUCTIONS");

    if (max_promotor != NULL && max_users != NULL && max_active_auctions != NULL) {
        printf("    > Environment variables detected, applying the following:\n");
        printf("        > max_promotor: %s, max_users: %s, max_active_auctions: %s\n", max_promotor, max_users,
               max_active_auctions);
        aux_numeric_env[0] = atoi(max_promotor);
        aux_numeric_env[1] = atoi(max_users);
        aux_numeric_env[2] = atoi(max_active_auctions);
    } else {
        perror(err_msg);
        aux_numeric_env[0] = 0;
        aux_numeric_env[1] = 0;
        aux_numeric_env[2] = 0;
    }

    char *f_promoters = getenv("FPROMOTERS");
    char *f_users = getenv("FUSERS");
    char *f_items = getenv("FITEMS");
    if (f_promoters != NULL && f_users != NULL && f_items != NULL) {
        file_environment_variables = true;
        printf("        > f_promoters: %s, f_users: %s, f_items: %s\n", f_promoters, f_users, f_items);
        aux_string_env[0] = f_promoters;
        aux_string_env[1] = f_users;
        aux_string_env[2] = f_items;
    }

    char* keep_alive_frontend = getenv("KEEPALIVE_FRONTEND");
    if(keep_alive_frontend != NULL){
        printf("        > keep_alive_frontend: %s\n ",keep_alive_frontend);
        aux_numeric_env[3] = atoi(keep_alive_frontend);
    } else {
        perror(err_msg);
        aux_numeric_env[3] = 0;
    }

    if (file_environment_variables) {
        config->max_promotors_allowed = aux_numeric_env[0];
        config->max_users_allowed = aux_numeric_env[1];
        config->max_auctions_active = aux_numeric_env[2];

        config->frontend_keep_alive_seconds = aux_numeric_env[3];

        config->f_promotors = aux_string_env[0];
        config->f_users = aux_string_env[1];
        config->f_items = aux_string_env[2];


        printf("    > environment variables defined properly\n");

    } else {
        printf("    > failed to initialize the backend application\n");
        printf("    > the file variables are required for initialization!\n");
        exit(1);
    }

    return config;
}
