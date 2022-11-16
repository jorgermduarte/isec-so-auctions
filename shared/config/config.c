#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

Config *get_env_variables() {
    Config *config = malloc(sizeof(Config));
    bool environment_variables_1 = false;
    bool environmment_variables_2 = false;

    int aux_numeric_env[3];
    char *aux_string_env[3];
    char err_msg[256] = "\n Environment variable missing. Assuming fallback value of 10";

    char* max_promotor = getenv("MAX_PROMOTOR");
    char* max_users = getenv("MAX_USERS");
    char* max_active_auctions = getenv("MAX_ACTIVE_AUCTIONS");

    if(max_promotor != NULL && max_users != NULL && max_active_auctions != NULL){
        environment_variables_1 = true;
        printf("    > Environment variables detected, applying the following:\n");
        printf("        > max_promotor: %s, max_users: %s, max_active_auctions: %s\n",max_promotor,max_users,max_active_auctions);
        aux_numeric_env[0] = atoi(max_promotor);
        aux_numeric_env[1] = atoi(max_users);
        aux_numeric_env[2] = atoi(max_active_auctions);
    }else{
        perror(err_msg);
        aux_numeric_env[0] = 0;
        aux_numeric_env[1] = 0;
        aux_numeric_env[2] = 0;
    }

    char* f_promoters = getenv("FPROMOTERS");
    char* f_users = getenv("FUSERS");
    char* f_items = getenv("FITEMS");
    if(f_promoters != NULL && f_users != NULL && f_items != NULL){
        environmment_variables_2 = true;
        printf("        > f_promoters: %s, f_users: %s, f_items: %s\n",f_promoters,f_users,f_items);
        aux_string_env[0] = f_promoters;
        aux_string_env[1] = f_users;
        aux_string_env[2] = f_items;
    }

    if(environmment_variables_2){
        config->max_promotors_allowed = aux_numeric_env[0];
        config->max_users_allowed = aux_numeric_env[1];
        config->max_auctions_active = aux_numeric_env[2];


        config->f_promotors = aux_string_env[0];
        config->f_users = aux_string_env[1];
        config->f_items = aux_string_env[2];

        printf("    > environment variables defined properly\n");

    }else{
        printf("    > failed to initialize the backend application\n");
        printf("    > the file variables are required for initialization!\n");
        exit(1);
    }

    return config;
}

/*

int main(){
    Config* conf = get_env_variables();

    int result = loadUsersFile(conf->f_users);
    if (result != -1)
        printf("\nUsers read: %d\n", result);

    char * username = "Alex";
    char * password = "123qwe.";
    int login = isUserValid(username, password);

    if (login == -1) {
        perror("\nError validating user from file");
        exit(1);
    }
    else if (login == 0) {
        perror("\nUser doesn't exist or his password is invalid");
        exit(1);
    }
    printf("\nUser %s with password %s is logged in\n", username, password);


    printf("\n%s balance is %d €\n", username, getUserBalance(username));
    printf("\nAdd 200 to balance...\n");
    if  (updateUserBalance(username, getUserBalance(username) + 200) == -1)
        printf("\n%s balance is %d €\n", username, getUserBalance(username));
    else {
        perror("Error updating balance\n");

        printf("%s\n", getLastErrorText());
        exit(1);
    }

    int was_saved = saveUsersFile(conf->f_users);
    if  (was_saved == 0) {
        perror("File saved\n");
    }else {
        perror("Error updating file\n");

        printf("%s\n", getLastErrorText());
        exit(1);
    }

    return 1;
}
*/
