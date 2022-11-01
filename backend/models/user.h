#include <stdio.h>
#include "item.h"

typedef struct User {
	char[50] username;
	char[50] password;
	int budget;
	Item[10] items_for_sale;
	Item[10] items_bought;
} User;