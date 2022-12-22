#include "commands_handler.h"

void command_arguments_display(struct string_list *head) {
    struct string_list *ptr = head;
    while (ptr) {
        printf("    >> Argument defined: %s\n", ptr->string);
        ptr = ptr->next;
    }
}

struct string_list *get_argument_list_tail_pointer(struct string_list *head) {
    if (head == NULL)
        return NULL;
    else {
        struct string_list *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        return current;
    }
}

struct string_list *get_command_arguments(char *command) {
    int arguments_number = 0;
    struct string_list *head = NULL;

    char *delimiter = " ";
    char *argument = strtok(command, delimiter);

    //printf("    > Arguments received:\n");
    while (argument != NULL) {
        //verify  the max arguments that we want to grab from the input
        if (arguments_number < MAX_ARGUMENTS_COMMAND) {
            //printf("        - %s\n", argument);

            if (head == NULL) {
                head = malloc(sizeof(struct string_list));
                head->string = malloc(strlen(argument) + 1);
                head->next = NULL;
                strcpy(head->string, argument);
            } else {
                struct string_list *new_argument = malloc(sizeof(struct string_list));
                new_argument->string = malloc(strlen(argument) + 1);
                strcpy(new_argument->string, argument);
                new_argument->next = NULL;

                struct string_list *tail = get_argument_list_tail_pointer(head);
                if (tail != NULL) {
                    tail->next = new_argument;
                }
            }
        } else {
            break;
        }
        arguments_number++;
        argument = strtok(NULL, delimiter);
    }

    return head;

}

int command_validate_name(char *command, char *target) {
    //printf(" -> comparing string %s with %s",command,target);
    if (strcmp(command, target) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int verify_is_number(char *t) {
    int is_number = 1;
    for (t; *t != '\0'; t++) {
        if (!isdigit(*t)) {
            is_number = 0;
        }
    }

    return is_number;
}

void clean_linked_list(struct string_list *list) {
    struct string_list *p;
    while (list != NULL) {
        p = list;
        free(list->string);
        list = list->next;
        free(p);
    }
}