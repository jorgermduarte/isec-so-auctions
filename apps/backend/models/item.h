#include <stdio.h>

typedef struct Item {
    char identifier[20];
	char seller_name[50];
	char bidder_name[20];
	char name[20];
	char category[20];
	int current_value;
	int buy_now_value;
	int duration;
	int active;
} Item;