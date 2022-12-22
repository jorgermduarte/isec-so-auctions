#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARGUMENTS_COMMAND 10

struct string_list{
    char* string;
    struct string_list* next;
};

/**
 * Gets all command arguments passed
 *
 * @param command
 * @return struct string_list* arguments
 */
struct string_list* get_command_arguments(char *command);

/**
 * Displays the arguments present in a given string_list
 *
 * @param struct string_list* head
 */
void command_arguments_display(struct string_list* head);

/**
 * Initiates the command handler
 */
void command_handler_start();

/**
 * Verifies if a given character is a number
 *
 * @param t
 * @return int
 */
int verify_is_number(char* t);

/**
 * Validates the command name passed
 *
 * @param command
 * @param target
 * @return int
 */
int command_validate_name(char* command, char* target);

/**
 * Cleans the linked list
 *
 * @param list
 */
void clean_linked_list(struct string_list* list);


//TODO:implement a method to clean the memory from the linked list since we are using mallocs etc
