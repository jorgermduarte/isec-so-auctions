#include <stdlib.h>
#include <string.h>
#define MAX_ARGUMENTS_COMMAND 5

struct string_list{
    char* string;
    struct string_list* next;
};

struct string_list* get_command_arguments(char *command);
void command_arguments_display(struct string_list* head);
int command_try_execution(char* command, struct string_list* arguments);
void command_handler_start();
