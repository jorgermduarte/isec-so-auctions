#include "./commands.h"

/**
 * Method that tries to execute a command with the given parameters
 *
 * @param command
 * @param arguments
 * @return int
 */
int command_try_execution(char *command, struct string_list *arguments);

/**
 * Initiates the command handler
 */
void command_handler_start();