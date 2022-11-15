#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARGUMENTS_COMMAND 5

struct string_list{
    char* string;
    struct string_list* next;
};

struct string_list* get_command_arguments(char *command);
void command_arguments_display(struct string_list* head);
void command_handler_start();
int verify_is_number(char* t);
int command_validate_name(char* command, char* target);

//TODO:implement a method to clean the memory from the linked list since we are using mallocs etc
