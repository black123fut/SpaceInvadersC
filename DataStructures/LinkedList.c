#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "LinkedList.h"

void createList(struct LinkedList *list, int elementSize, freeFunction fn) {
    assert(elementSize > 0);
    list->head = NULL;
    list->tail = NULL;
    list->elementSize = elementSize;
    list->freeFn = fn;
    list->size = 0;
}

void add(struct LinkedList *list, void *data) {
    Node *tmp = (Node *) malloc(sizeof(Node));
    tmp->data = malloc(sizeof(list->elementSize));
    tmp->next = NULL;

    memcpy(tmp->data, data, list->elementSize);

    if (list->size == 0) {
        list->head = tmp;
        list->tail = tmp;
    }
    else {
        list->tail->next = tmp;
        list->tail = tmp;
    }
    list->size++;
}

void delete_node(struct LinkedList *list, int index) {
    if (index < list->size) {
        if (index == 0) {
            Node *tmp = list->head;
            list->head = list->head->next;
            free(tmp->data);
            free(tmp);
        }
        else {
            Node *tmp = list->head;
            for (int i = 1; i < list->size; ++i) {
                if (i == index) {
                    Node *tmp2 = tmp->next;
                    tmp->next = tmp->next->next;
                    free(tmp2->data);
                    free(tmp2);
                    break;
                }
                tmp = tmp->next;
            }
        }
        list->size--;
    }
    else {
        printf("delete: Index out of range");
    }
}

void *get(struct LinkedList *list, int index) {
    if (list->size > index) {
        Node *tmp = list->head;
        for (int i = index; i != 0; --i) {
            tmp = tmp->next;
        }
        return tmp->data;
    }
    else {
        printf("get: Index out of range");
        return NULL;
    }
}

int length(struct LinkedList *list) {
    return list->size;
}

void list_destroy(struct LinkedList *list)
{
    Node *current;
    while(list->head != NULL) {
        current = list->head;
        list->head = current->next;

        if(list->freeFn) {
            list->freeFn(current->data);
        }
        free(current->data);
        free(current);
    }
}