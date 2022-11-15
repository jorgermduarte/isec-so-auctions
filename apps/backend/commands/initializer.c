#include "initializer.h"

int command_try_execution(char* command, struct string_list* arguments){
    printf("    > Verifying command existence\n");
    int exit = 0;

    if(command_validate_name(command,"list")){
        exec_command_list();
    }else if(command_validate_name(command,"users")){
        exec_command_list_users();
    }else if(command_validate_name(command,"kick")){
        exec_command_kick(arguments);
    }else if(command_validate_name(command,"prom")){
        exec_command_prom();
    }else if(command_validate_name(command,"reprom")){
        exec_command_reprom();
    }else if(command_validate_name(command,"cancel")){
        exec_command_cancel_prom(arguments);
    }else if(command_validate_name(command,"testlib")){
        test_users_lib();
    }else if(command_validate_name(command,"close")){
        printf("     > closing the application...\n");
        //TODO: notify the frontend applications that the backend is closing
        exit = 1;
    }else{
        printf("     > Couldn't find any command for: %s\n", command);
    }
    return exit;
}

void command_handler_start(){
    size_t bufsize = 255;
    char buffer[bufsize];

    printf(" > Command: ");
    fgets(buffer,bufsize,stdin);
    printf("    > Command received: %s", buffer);

    size_t len = strlen(buffer);

    if(len > 1){
        //remove a possible added \n
        if (buffer[len - 1] == '\n') {  // FAILS when len == 0
            buffer[len -1] = '\0';
        }

        struct string_list* arguments = get_command_arguments(buffer);

        command_arguments_display(arguments);
        int exit = command_try_execution(arguments->string,arguments->next);
        if(!exit){
            clean_linked_list(arguments);
            command_handler_start();
        }
    }else{
        printf("    > Invalid command provided, please try again..\n");
        command_handler_start();
    }
}
