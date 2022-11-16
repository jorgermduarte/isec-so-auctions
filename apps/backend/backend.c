#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../shared/helpers/constants.h"
#include "../../shared/helpers/helpers.h"
#include "./commands/initializer.h"
#include "models/backend.h"

Item* load_items_from_file(char *filename) {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total = 0;
    Item *item_list;

    file = fopen(filename, "r");
    if (NULL == file) {
        perror("\nError opening items file. Exiting with status");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        size_t len = strlen(line);

        if (len > 1) {
            //remove a possible added \n
            if (line[len - 1] == '\n') {  // FAILS when len == 0
                line[len - 1] = '\0';
            }
        }

        struct string_list *arguments = get_command_arguments(line);

        if (arguments != NULL) {

            Item *item = malloc(sizeof(Item));
            
            set_attribute(item->identifier, arguments->string);
            set_attribute(item->name, arguments->next->string);
            set_attribute(item->category,arguments->next->next->string );
            set_attribute(item->seller_name, arguments->next->next->next->next->next->next->string);
            set_attribute(item->bidder_name,arguments->next->next->next->next->next->next->next->string );
            // item->identifier = malloc(strlen(arguments->string) + 1);
            // item->name = malloc(strlen(arguments->next->string) + 1);
            // item->category = malloc(strlen(arguments->next->next->string) + 1);
            // item->seller_name = malloc(strlen(arguments->next->next->next->next->next->next->string) + 1);
            // item->bidder_name = malloc(strlen(arguments->next->next->next->next->next->next->next->string) + 1);

            // strcpy(item->identifier, arguments->string);
            // strcpy(item->name, arguments->next->string);
            // strcpy(item->category,arguments->next->next->string);
            // strcpy(item->seller_name, arguments->next->next->next->next->next->next->string);
            // strcpy(item->bidder_name, arguments->next->next->next->next->next->next->next->string);

            item->current_value = atoi(arguments->next->next->next->string);
            item->buy_now_value = atoi(arguments->next->next->next->next->string);
            item->duration = atoi(arguments->next->next->next->next->next->string);

            total++;
            item_list = realloc(item_list, sizeof(struct Item) * total);
            item_list[total - 1] = *item;

        } else {
            printf("  > blank line detected ...! \n");
        }

        // clean the arguments list memory
        clean_linked_list(arguments);
    }

    for (int i = 0;i < total; i++) {
        Item current = item_list[i];
        printf("    -> identifier: %s, bidder name: %s, current value: %d \n", current.identifier, current.bidder_name,
               current.current_value);
    }

    fclose(file);
    return item_list;
}

User *load_users_from_file(char *filename) {
    // TODO: import library from professor and read file name, importing users

    return NULL;
}

Promotor *load_promotors_from_file(char *filename) {
    // TODO: import library from professor and read file name, importing promotors

    return NULL;
}

Backend *bootstrap() {
    Backend *app = malloc(sizeof(Backend));

    Config *config = get_env_variables();

    app->config = config;
    app->items = load_items_from_file(app->config->f_items);
    pthread_create(&app->threads.pthread_backend_commands, NULL, command_thread_handler, &app);

    return app;
}

void *command_thread_handler(void *pdata) {
    //TODO: right now we are not doing anything with the Backend app structure
    Backend *app = (Backend *) pdata;
    //initialize the logic to receive command inputs
    command_handler_start();
}


void clean_item(Item* item){
    free(item->bidder_name);
    free(item->seller_name);
    free(item->category);
    free(item->identifier);
    free(item->name);
    free(item);
}

//TODO: Not sure if it is right (verify later)
// void clean_list_item(Item* item){
//     while(item != NULL){
//         clean_item(item);
//         item++;
//     }
//}