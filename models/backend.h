#include <stdio.h>
#include "../models/item.h"
#include "../models/user.h"
#include "../models/promotor.h"
#include "../models/auction.h"

typedef struct Backend
{
    Auction* auctions;
    Promotor* promotors;
    User* users;
} Backend;


Item *loadItemsFromFile( char *filename );
User *loadUsersFromFile( char *filename );
Promotor *loadPromotorsFromFile( char* filename );

Backend* bootstrap();