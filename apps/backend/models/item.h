#include <stdio.h>

typedef struct Item {
    char *identifier;
	char *name;
	char *category;
	int current_value;
	int buy_now_value;
	int duration;
	char *seller_name;
	char *bidder_name;
} Item;