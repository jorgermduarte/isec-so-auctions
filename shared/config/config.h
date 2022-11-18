typedef struct Config {
    int max_users_allowed;
    int max_promotors_allowed;
    int max_auctions_active;
    char *f_promotors;
    char *f_users;
    char *f_items;
} Config;


/**
 * Method that gets the specific environment variables used in the project
 *
 * @return Config* configuration_data
 */
Config *get_env_variables();

