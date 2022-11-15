#include "initializer.h"

int command_try_execution(char* command, struct string_list* arguments){
    printf("    > Verifying command existence\n");
    int exit = 0;

    if(command_validate_name(command,"list")){
        list_all_items(arguments);
    }else if(command_validate_name(command,"users")){
        list_users(arguments);
    }else if(command_validate_name(command,"kick")){
        kick_user(arguments);
    }else if(command_validate_name(command,"prom")){
        list_promotors(arguments);
    }else if(command_validate_name(command,"reprom")){
        update_promotors(arguments);
    }else if(command_validate_name(command,"cancel")){
        cancel_promotor(arguments);
    }else if(command_validate_name(command,"close")){
        close_platform(arguments);
    }else if(command_validate_name(command,"exit")){
        printf("     > closing the application...\n");
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

    //remove a possible added \n
    size_t len = strlen(buffer);
    if (buffer[len - 1] == '\n') {  // FAILS when len == 0
        buffer[len -1] = '\0';
    }

    struct string_list* arguments = get_command_arguments(buffer);

    command_arguments_display(arguments);
    int exit = command_try_execution(arguments->string,arguments->next);
    if(!exit)
        command_handler_start();
}
