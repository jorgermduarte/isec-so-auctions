#include "./commands.h"
#include "../models/backend.h"


int command_try_execution(char* command, struct string_list* arguments, int pid_response);

void command_handler_start(Backend* app);