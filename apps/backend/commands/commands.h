#include "../../../shared/commands/commands_handler.h"

//list all items
void exec_command_list(int pid_response);

void exec_command_list_users();

void exec_command_kick(struct string_list* arguments);

//list promoters
void exec_command_prom();

//update promoters
void exec_command_reprom();

//cancel promoters
void exec_command_cancel_prom(struct string_list* arguments);

// function used to display the tests of the user lib
void test_users_lib();