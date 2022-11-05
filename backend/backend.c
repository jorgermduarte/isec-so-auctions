#include "../models/backend.h"

Item *load_items_from_file( char *filename )
{
    // TODO: import library from professor and read file name, importing items
    
    return NULL;
}

User *load_users_from_file( char *filename ) 
{
    // TODO: import library from professor and read file name, importing users
    
    return NULL;
}

Promotor *load_promotors_from_file( char* filename ) 
{
    // TODO: import library from professor and read file name, importing promotors
    
    return NULL;
}

Backend* bootstrap()
{
    Backend backend;
    malloc( &backend, sizeof(Backend) );

    Config config = get_env_variables();

    backend->config = config;
    backend->users = load_users_from_file( "put_filename_here" );
    backend->promotors = load_promotors_from_file( "put_filename_here" );

    // TODO: finish this logic setting up every structure for the functioning of backend, 
    // this must be an adaptation of a singleton class
}

void *command_thread_handler( void *pdata ) 
{
    Backend *backend = (Backend*)pdata;
    
}