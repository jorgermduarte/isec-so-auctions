#include <stdio.h>
#include "./commands_handler.h"

int main( int argc, char* argv[] )
{
    if(argc != 3){
        printf("Please provide the username and password when initializing the client(frontend) application.\n");
        printf("Example: ./client manuel password_do_manuel\n");
    }else{
        printf("======================= USER PROVIDED ======================== \n");
        char* username = argv[1];
        char* password = argv[2];
        printf("username: %s\n",username);
        printf("password: *******\n");
        printf("========================== COMMANDS ========================== \n");
        command_handler_start();
    }

	return 0;
}
