typedef struct Config {
    int max_users_allowed;
    int max_promotors_allowed;
    int max_auctions_active;
} Config;


Config *get_env_variables();

