#include <stdio.h>
#include "./commands_handler.h"

int main( int argc, char* argv[] )
{
    //TODO: Implement the logic to grab the client credentials from the argv array
    //validate at least the credentials provided (required values) username && password
    // ./frontend manuel password_do_manuel

    printf("========================== COMMANDS ========================== \n");
    command_handler_start();
	return 1;
}
