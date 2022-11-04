#include <stdio.h>

typedef struct Item {
	char name[50];
	char category[50];
	int current_value;
	int buy_now_value;
	int duration;
	char seller_name[50];
	char bidder_name[50];
} Item;