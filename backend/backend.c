#include "../models/backend.h"

Item *loadItemsFromFile( char *filename )
{
    // TODO: import library from professor and read file name, importing items
    
    return NULL;
}

User *loadUsersFromFile( char *filename ) 
{
    // TODO: import library from professor and read file name, importing users
    
    return NULL;
}

Promotor *loadPromotorsFromFile( char* filename ) 
{
    // TODO: import library from professor and read file name, importing promotors
    
    return NULL;
}

Backend* bootstrap()
{
    Backend backend;
    malloc(&backend, sizeof(Backend));

    backend->users = loadUsersFromFile("put_filename_here");
    backend->promotors = loadPromotorsFromFile("put_filename_here");

    // TODO: finish this logic setting up every structure for the functioning of backend, 
    // this must be an adaptation of a singleton class
}