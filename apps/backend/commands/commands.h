#include "../../../shared/commands/commands_handler.h"

struct Backend;
// ======== ONLY FRONTEND COMMANDS =========
void exec_command_verify_login(struct string_list* arguments, struct Backend* app, int pid_response);
void exec_command_cash(struct Backend* app, int pid_response);
void exec_add_money_to_user(struct Backend* app,  int pid_response, struct string_list* arguments);
void exec_command_time(struct Backend* app, int pid_response);
void exec_command_exit_frontend(struct Backend* app, int pid_response);
void exec_command_licat(struct Backend* app, int pid_response, struct string_list* arguments);
void exec_command_litime(struct Backend* app, int pid_response, struct string_list* arguments);
void exec_command_lival(struct Backend* app, int pid_response, struct string_list* arguments);
void exec_command_lisel(struct Backend* app, int pid_response, struct string_list* arguments);
void exec_command_sell(struct Backend* app, int pid_response, struct string_list* arguments);
void exec_command_buy(struct Backend* app, int pid_response, struct string_list* arguments);

// ======== ONLY BACKEND COMMANDS =========
// list all items from the database / file
void exec_command_list(struct Backend *app,int pid_response);
// list all users from the file
void exec_command_list_users(struct Backend* app);
// list promoters
void exec_command_prom(struct Backend* app);

// TODO: commands to implement from the backend application
void exec_command_kick(struct Backend* app, struct string_list* arguments);
// update promoters
void exec_command_reprom();
// cancel promoter
void exec_command_cancel_prom(struct string_list* arguments);

// ======== BOTH FRONTEND AND BACKEND COMMANDS =========




