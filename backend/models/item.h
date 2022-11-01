#include <stdio.h>

typedef struct Item {
	char[50] name;
	char[50] category;
	int current_value;
	int buy_now_value;
	int duration;
	char[50] seller_name;
	char[50] bidder_name;
} Item;