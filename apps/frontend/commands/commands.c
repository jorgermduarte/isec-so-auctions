#include "./commands.h"

void exec_command_list() {
    printf("     > Executing the list command\n");
}

void exec_command_licat(struct string_list *arguments) {
    if (arguments != NULL) {
        printf("     > Executing the licat command, with the category: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the licat command, please provide the category name, example: licat category-name\n");
    }
}

void exec_command_lisel(struct string_list *arguments) {
    if (arguments != NULL) {
        printf("     > Executing the lisel command, with the seller name: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the lisel command, please provide the category name, example: lisel seller-name\n");
    }
}

void exec_command_lival(struct string_list *arguments) {
    if (arguments != NULL && verify_is_number(arguments->string)) {
        printf("     > Executing the lival command, with the value: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the lival command, please provide a value, example: lival 30\n");
    }
}

void exec_command_litime(struct string_list *arguments) {
    if (arguments != NULL && verify_is_number(arguments->string)) {
        printf("     > Executing the litime command, with the value: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the litime command, please provide a value, example: litime 30\n");
    }
}

void exec_command_time() {
    printf("     > Executing the time command\n");
}

void exec_command_buy(struct string_list *arguments) {
    if (arguments != NULL && arguments->next != NULL && verify_is_number(arguments->string) &&
        verify_is_number(arguments->next->string)) {
        printf("     > Executing the buy command, with the values: id: %s , value: %s\n", arguments->string,
               arguments->next->string);
    } else {
        printf("     > Failed to execute the buy command, please provide two values, example: buy <id> <value>\n");
    }
}

void exec_command_cash() {
    printf("     > Executing the cash command\n");
}

void exec_command_add(struct string_list *arguments) {
    if (arguments != NULL && verify_is_number(arguments->string)) {
        printf("     > Executing the add command, with the value: %s\n", arguments->string);
    } else {
        printf("     > Failed to execute the add command, please provide a value, example: add 30\n");
    }
}