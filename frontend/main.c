#include <stdio.h>
#include "../helpers/helpers.h"
#include "../helpers/constants.h"
#include "../commands/commands.h"


int main( int argc, char* argv[] )
{

	// todo: migrate this logic to a method in the future. 
	// for now this just needs to validate input syntax in every method.
	// JOKA take this from here if you want! 
	char command[CMD_BUF_SIZE];

    while (1)
    {
        scanf("$> %s", command);
        int cmdLen = strlen(command);

        if (contains(command, "list")) {
            list_all_items(app, command);
        } else if (contains(command, "licat")) {
            list_all_items_from_category(app, command);
        } else if (contains(command, "lisel")) {
            list_all_items_by_seller(app, command);
        } else if (contains(command, "lival")) {
            list_all_items_by_price(app, command);
        } else if (contains(command, "litime")) {
            list_all_items_by_due_time(app, command);
        } else if (contains(command, "buy")) {
            bid_item(app, command);
        } else if (contains(command, "time")) {
            get_current_time(app, command);
        } else if (contains(command, "cash")) {
            get_current_budget(app, command);
        }
        
    }


	return 1;
}