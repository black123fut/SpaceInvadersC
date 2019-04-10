#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "LinkedList.h"

void createList(struct LinkedList *list, int elementSize, freeFunction fn) {
    assert(elementSize > 0);
    list->head = NULL;
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

    }
    else {
        Node *sub = list->head;
        while (sub->next != NULL) {
            sub = sub->next;
        }
        sub->next = tmp;
    }
    list->size++;
}

void delete_node(struct LinkedList *list, int index, char *tag) {
    int counter = 0;
    Node *tmp = list->head;

    if (list->head == NULL) {
        return;
    }
    if (index == 0) {
        list->head = list->head->next;
        if(list->freeFn) {
            list->freeFn(tmp->data);
        }
        free(tmp->data);
        free(tmp);
        list->size--;
        return;
    }
    counter++;
    while(tmp->next != NULL) {
        if (counter == index) {

            Node *tmpp = tmp->next;
            tmp->next = tmp->next->next;
            if(list->freeFn) {
                list->freeFn(tmpp->data);
            }
            free(tmpp->data);
            free(tmpp);
            list->size--;
            return;
        }
        else {
            tmp = tmp->next;
            counter++;
        }
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

void clear_list(struct LinkedList *list) {
    int size = length(list);
    for (int i = 0; i < size; ++i) {
        delete_node(list, 0, "");
    }
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