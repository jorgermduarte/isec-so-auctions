#include <stdio.h>
#include "models/promotions.h"
#include <stdlib.h>

struct Promotions* getTailPromotions(struct Promotions *head){
    struct Promotions *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

struct Promotions* getHeadPromotions(struct Promotions *tail){
    struct Promotions *current = tail;
    while (current->prev != NULL) {
        current = current->prev;
    }
    return current;
}

struct Promotions* getPromotionById(struct Promotions *head, int id){
    struct Promotions *current = head;
    while (current->next != NULL) {
        if(current->id == id){
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void addPromotion(struct Promotions *head, struct Promotions *promotion){
    if(head == NULL){
        head = promotion;
    }else{
        struct Promotions *tail = getTailPromotions(head);
        tail->next = promotion;
        promotion->prev = tail;
        promotion->next = NULL;
    }
}

void removePromotion(struct Promotions *head, int id){
    struct Promotions *current = getPromotionById(head, id);
    if(current != NULL){
        if(current->prev != NULL){
            current->prev->next = current->next;
        }
        if(current->next != NULL){
            current->next->prev = current->prev;
        }
        free(current);
    }
}

void updatePromotion(struct Promotions *head, int id, struct Promotions *promotion){
    struct Promotions *current = getPromotionById(head, id);
    if(current != NULL){
        current->time = promotion->time;
        current->value = promotion->value;
        current->valid = promotion->valid;
    }
}
