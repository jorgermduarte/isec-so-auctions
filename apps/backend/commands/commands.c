#include "./commands.h"
#include "../../../shared/helpers/helpers.h"
#include "../config/config.h"
#include "../../../users_lib.h"
#include "../models/item.h"
#include "../notifier.h"

//list all items
void exec_command_list(int pid_response) {
    printf("     > Executing the list all items command\n");
    bool sendMessagesFrontend = false;
    if(pid_response != -1) {
        sendMessagesFrontend = true;
    }
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    fp = fopen("file_items.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        Item it;
        // the %20s is to avoid buffer overflows
        sscanf(line, "%20s %20s %20s %d %d %d %20s %20s", it.identifier, it.name, it.category, &it.duration, &it.current_value, &it.buy_now_value, it.seller_name, it.bidder_name);

        printf("     > %s %s %s %d %d %d %s %s\n", it.identifier, it.name, it.category, it.duration, it.current_value, it.buy_now_value, it.seller_name, it.bidder_name);

        if(sendMessagesFrontend){

            char message_to_send[255] = "";
            strcat(message_to_send, it.identifier);
            strcat(message_to_send, " ");
            strcat(message_to_send, it.name);
            strcat(message_to_send, " ");
            strcat(message_to_send, it.category);
            strcat(message_to_send, " ");
            strcat(message_to_send, it.seller_name);

            send_message_frontend(message_to_send, pid_response);
        }

    }

    fclose(fp);
    if (line)
        free(line);
}

void exec_command_list_users() {
    printf("     > Executing the list users command\n");
}

void exec_command_kick(struct string_list *arguments) {
    if (arguments != NULL) {
        printf("     > Executing the kick user command, kicking the user: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the kick command, please provide the username, example: kick <username>\n");
    }
}

//list promoters
void exec_command_prom() {
    printf("     > Executing the list promoters command\n");
}

//update promoters
void exec_command_reprom() {
    printf("     > Executing the update promoters command\n");

}

//cancel promoter
void exec_command_cancel_prom(struct string_list *arguments) {
    if (arguments != NULL) {
        printf("     > Executing the cancel promoter command, canceling promoter: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the cancel promoter command, please provide the exec promoter name, example: cancel <exec-promoter-name>\n");
    }
}

// function used to display the tests of the user lib
//TODO: This command is here for test purposes and for the first meta
void test_users_lib() {
    bbash();
    printf("     > Executing the test users lib example: \n");

    Config *conf = get_env_variables();

    int result = loadUsersFile(conf->f_users);
    if (result != -1)
        printf("      > Users read: %d\n", result);

    char *username = "Alex";
    char *password = "123qwe.";
    int login = isUserValid(username, password);

    if (login == -1) {
        perror("      > Error validating user from file");
        exit(1);
    } else if (login == 0) {
        perror("      > User doesn't exist or his password is invalid");
        exit(1);
    }

    printf("      > User %s with password %s is logged in\n", username, password);
    printf("      > %s balance is %d €\n", username, getUserBalance(username));
    printf("      > Removing 1€ to balance...\n");
    if (updateUserBalance(username, getUserBalance(username) - 1) == -1)
        printf("      > %s balance is %d€\n", username, getUserBalance(username));
    else {
        perror("      > Error updating balance\n");
        printf("      > %s\n", getLastErrorText());
        exit(1);
    }

    int was_saved = saveUsersFile(conf->f_users);
    if (was_saved == 0) {
        printf("      > File saved\n");
    } else {
        perror("      > Error updating file\n");
        printf("      > %s\n", getLastErrorText());
        exit(1);
    }
    creset();
}
