#include "initializer.h"

int command_try_execution(char* command, struct string_list* arguments){
    printf("    > Verifying command existence\n");
    int exit = 0;

    if(command_validate_name(command,"list")){
        exec_command_list();
    }else if(command_validate_name(command,"licat")){
        exec_command_licat(arguments);
    }else if(command_validate_name(command,"lisel")){
        exec_command_lisel(arguments);
    }else if(command_validate_name(command,"lival")){
        exec_command_lival(arguments);
    }else if(command_validate_name(command,"litime")){
        exec_command_litime(arguments);
    }else if(command_validate_name(command,"buy")){
        exec_command_buy(arguments);
    }else if(command_validate_name(command,"time")){
        exec_command_time();
    }else if(command_validate_name(command,"cash")){
        exec_command_cash();
    }else if(command_validate_name(command,"add")){
        exec_command_add(arguments);
    }else if(command_validate_name(command,"exit")){
        printf("     > closing the application...\n");
        //todo: inform the backend that we are disconnecting
        exit = 1;
    }else{
        printf("     > Couldn't find any command for: %s\n", command);
    }
    return exit;
}

void command_handler_start(){
    size_t bufsize = 255;
    char buffer[bufsize];

    printf("We are listening to any commands that you may provide, just write them and press enter\n");
    fgets(buffer,bufsize,stdin);
    printf("> Command received: %s", buffer);

    size_t len = strlen(buffer);

    if(len > 1){
        //remove a possible added \n
        if (buffer[len - 1] == '\n') {  // FAILS when len == 0
            buffer[len -1] = '\0';
        }

        struct string_list* arguments = get_command_arguments(buffer);

        //command_arguments_display(arguments);
        int exit = command_try_execution(arguments->string,arguments->next);
        if(!exit){
            //clean the arguments memory allocated to avoid memory leaks
            clean_linked_list(arguments);
            command_handler_start();
        }
    }else{
        printf("> Invalid command provided, please try again..\n");
        command_handler_start();
    }
}
