#include <stdio.h>
#include "../models/backend.h"

// Client commands handlers
void sell_item_in_auction(Backend app, char *command);

void list_items_on_sale(Backend app, char *command);

void list_all_items(Backend app, char *command);

void list_all_items_from_category(Backend app, char *command);

void list_all_items_by_seller(Backend app, char *command);

void list_all_items_by_price(Backend app, char *command);

void list_all_items_by_due_time(Backend app, char *command);

void get_current_time(Backend app, char *command);

void bid_item(Backend app, char *command);

void get_current_budget(Backend app, char *command);

void add_budget(Backend app, char *command);

// Backend commands handler
void list_users(Backend app, char *command);

void kick_user(Backend app, char *command);

void list_promotors(Backend app, char *command);

void update_promotors(Backend app, char *command);

void cancel_promotor(Backend app, char *command);

void close_platform(Backend app, char *command);

// Promotor commands handler