#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGUMENTS_COMMAND 5

struct string_list{
    char* string;
    struct string_list* next;
};

void command_arguments_display(struct string_list* head) {
    struct string_list* ptr = head;
    while(ptr) {
        printf("    >> Argument defined: %s\n", ptr->string);
        ptr = ptr->next;
    }
}

struct string_list* get_argument_list_tail_pointer(struct string_list* head){
    if(head == NULL)
        return NULL;
    else{
        struct string_list *current = head;
        while(current->next != NULL){
            current = current->next;
        }
        return current;
    }
}

struct string_list* get_command_arguments(char *commmand){
    int arguments_number = 0;
    struct string_list* head = NULL;

    char *delimiter = " ";
    char *argument = strtok(commmand, delimiter);

    printf("    > Arguments received:\n");
    while (argument != NULL)
    {

        //TODO: clean the argument value, i noticed that the exit comand required the \n to work
        //this is probably applying to the last argument too and could cause problems in the future
        //soo basically we need to implement a "clean-up" function that removes the \n from the arguments

        //verify  the max arguments that we want to grab from the input
        if(arguments_number < MAX_ARGUMENTS_COMMAND){
            printf("        - %s\n", argument);

            if(head == NULL){
                head = malloc(sizeof(struct string_list));
                head->string = malloc(strlen(argument) + 1);
                head->next = NULL;
                strcpy(head->string, argument);
            }else{
                struct string_list* new_argument = malloc(sizeof(struct string_list));
                new_argument->string = malloc(strlen(argument) + 1);
                strcpy(new_argument->string, argument);
                new_argument->next = NULL;

                //printf("    -> setting the new child argument: %s\n", new_argument->string);
                struct string_list* tail = get_argument_list_tail_pointer(head);
                //printf("    -> current tail: %s\n", tail->string);
                if(tail != NULL){
                    tail->next = new_argument;
                }
            }
        }else {
            break;
        }
        arguments_number++;
        argument = strtok(NULL, delimiter);
    }

    return head;

}

int command_validate_name(char* command, char* target){
    //printf(" -> comparing string %s with %s",command,target);
    if(strcmp (command, target) == 0){
        return 1;
    }else{
        return 0;
    }
}

int command_try_execution(char* command, struct string_list* arguments){
    printf("    > Verifying command existence\n");
    int exit = 0;

    if(command_validate_name(command,"list")){
        printf("     > Executing the list command\n");
    }else if(command_validate_name(command,"licat")){
        printf("     > Executing the licat command\n");
    }else if(command_validate_name(command,"lisel")){
        printf("     > Executing the lisel command\n");
    }else if(command_validate_name(command,"lival")){
        printf("     > Executing the lival command\n");
    }else if(command_validate_name(command,"litime")){
        printf("     > Executing the litime command\n");
    }else if(command_validate_name(command,"buy")){
        printf("     > Executing the buy command\n");
    }else if(command_validate_name(command,"time")){
        printf("     > Executing the time command\n");
    }else if(command_validate_name(command,"cash")){
        printf("     > Executing the cash command\n");
    }else if(command_validate_name(command,"exit\n")){
        printf("     > closing the application...\n");
        exit = 1;
    }else{
        printf("     > Couldn't find any command for: %s\n", command);
    }
    return exit;
}

void command_handler_start(){
    char *buffer;
    size_t bufsize = 255; //TODO: replace 255 to CMD_BUF_SIZE
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    printf(" > Command: ");
    characters = getline(&buffer,&bufsize,stdin);
    printf("    > Command received: %s", buffer);

    struct string_list* arguments = get_command_arguments(buffer);

    command_arguments_display(arguments);
    int exit = command_try_execution(arguments->string,arguments->next);
    if(!exit)
        command_handler_start();
}

// TODO: migrate all the commands functions to the commands folder
int main( int argc, char* argv[] )
{
    printf("========================== COMMANDS ========================== \n");
    command_handler_start();
	return 1;
}
