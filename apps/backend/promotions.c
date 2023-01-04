#include <stdio.h>
#include "models/promotions.h"
#include <stdlib.h>
#include <string.h>

struct Promotions *getTailPromotions(struct Promotions *head)
{
    struct Promotions *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    return current;
}

struct Promotions *getHeadPromotions(struct Promotions **tail)
{
    struct Promotions *current = *tail;
    while (current != NULL)
    {
        current = current->prev;
    }
    return current;
}

struct Promotions *getPromotionById(struct Promotions **head, int id)
{
    struct Promotions *current = *head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct Promotions *getPromotionByCategory(struct Promotions **head, char *category)
{
    struct Promotions *current = *head;
    while (current != NULL)
    {
        if (strcmp(current->category, category) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addPromotion(struct Promotions **head, struct Promotions *promotion)
{
    if (*head == NULL)
    {
        *head = promotion;
    }
    else
    {
        struct Promotions *tail = getTailPromotions(*head);
        if (tail != NULL)
        {
            tail->next = promotion;
            promotion->prev = tail;
            promotion->next = NULL;
        }
    }
}

void removePromotion(struct Promotions **head, int id)
{
    struct Promotions *current = *head;
    while (current != NULL)
    {
        if (current->id == id)
        {

            if (current->prev == NULL && current->next == NULL) // there is no more promotions ( only one);
            {
                *head = NULL;
            }else if (current->prev == NULL && current->next != NULL) // the first promotion but have more in the list
            {
                *head = current->next;
                current->next->prev = NULL; // set the prev of the next promotion to NULL
            }else if (current->next != NULL && current->prev != NULL) // if is in the middle
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            else if( current->prev != NULL && current->next == NULL) // if is the last promotion
            {
                current->prev->next = NULL;
            }

            free(current);
            break;
        }
        current = current->next;
    }
}

void updatePromotion(struct Promotions **head, int id, struct Promotions *promotion)
{
    struct Promotions *current = *head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            strcpy(current->category, promotion->category);
            current->time = promotion->time;
            current->value = promotion->value;
            current->valid = promotion->valid;
            break;
        }
        current = current->next;
    }
}

void displayPromotions(struct Promotions *head)
{
    struct Promotions *current = head;

    if (current == NULL)
    {
        printf("No promotions found to list\n");
    }
    else
    {
        while (current != NULL)
        {
            printf("    > id: %d - category: %s - time: %d - value: %d - valid: %d \n ", current->id, current->category,
                   current->time, current->value, current->valid);
            current = current->next;
        }
    }
}