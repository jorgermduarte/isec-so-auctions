#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "../../users_lib.h"

Config *get_env_variables() {
    Config *config = malloc(sizeof(Config));
    int aux_numeric_env[3];
    char *aux_string_env[3];

    char err_msg[256] = "\n Environment variable missing. Assuming fallback value of 10";

    aux_numeric_env[0] = atoi(getenv("MAX_PROMOTOR"));
    aux_numeric_env[1] = atoi(getenv("MAX_USERS"));
    aux_numeric_env[2] = atoi(getenv("MAX_ACTIVE_AUCTIONS"));

    aux_string_env[0] = getenv("FPROMOTERS");
    aux_string_env[1] = getenv("FUSERS");
    aux_string_env[2] = getenv("FITEMS");

    for (int i = 0; i < 3; i++) {
        if (aux_numeric_env[i] == 0) {
            perror(err_msg);
            aux_numeric_env[i] = 10;
        }
    }

    config->max_promotors_allowed = aux_numeric_env[0];
    config->max_users_allowed = aux_numeric_env[1];
    config->max_auctions_active = aux_numeric_env[2];

    config->f_promotors = aux_string_env[0];
    config->f_users = aux_string_env[1];
    config->f_items = aux_string_env[2];

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
