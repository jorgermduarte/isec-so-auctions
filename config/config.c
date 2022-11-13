#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"


Config *get_env_variables() 
{
    Config *config = malloc(sizeof(Config));
    int aux_env[3];
    char err_msg[256] = "\n Environment variable missing. Assuming fallback value of 10";

    aux_env[0] = atoi(getenv("MAX_PROMOTOR"));
    aux_env[1] = atoi(getenv("MAX_USERS"));
    aux_env[2] = atoi(getenv("MAX_ACTIVE_AUCTIONS"));

    for (int i = 0; i < 3; i++)
    {
        if (aux_env[i] == 0){
            perror(err_msg);
            aux_env[i] = 10;        
        }
    }
      

    config->max_promotors_allowed = aux_env[0];
    config->max_users_allowed = aux_env[1];
    config->max_auctions_active = aux_env[2];
    
    return config;
}
