#include <stdio.h>
#include "models/promotions.h"
#include <stdlib.h>
#include <string.h>

struct Promotions* getTailPromotions(struct Promotions *head){
    struct Promotions *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

struct Promotions* getHeadPromotions(struct Promotions **tail){
    struct Promotions *current = *tail;
    while (current != NULL) {
        current = current->prev;
    }
    return current;
}

struct Promotions* getPromotionById(struct Promotions **head, int id){
    struct Promotions *current = *head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addPromotion(struct Promotions **head, struct Promotions *promotion){
    if (*head == NULL) {
        *head = promotion;
    } else {
        struct Promotions *tail = getTailPromotions(*head);
        if (tail != NULL) {
            printf(" adding child\n");
            tail->next = promotion;
            promotion->prev = tail;
        }
    }
}

void removePromotion(struct Promotions **head, int id){
    struct Promotions *current = *head;
    while (current != NULL) {
        if (current->id == id) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }

            if(current->prev == NULL && current->next == NULL){
                *head = NULL;
            }

            free(current);

            break;
        }
        current = current->next;
    }
}

void updatePromotion(struct Promotions **head, int id, struct Promotions *promotion){
    struct Promotions *current = *head;
    while (current != NULL) {
        if (current->id == id) {
            strcpy(current->category, promotion->category);
            current->time = promotion->time;
            current->value = promotion->value;
            current->valid = promotion->valid;
            break;
        }
        current = current->next;
    }
}

void displayPromotions(struct Promotions *head){
    struct Promotions *current = head;

    if(current == NULL){
        printf("No promotions found to list\n");
    }else{
        while(current != NULL) {
            printf("    > id: %d - category: %s - time: %d - value: %d - valid: %d \n ", current->id, current->category,
                   current->time, current->value, current->valid);
            current = current->next;
        }
    }
}
