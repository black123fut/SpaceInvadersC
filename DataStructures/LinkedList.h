#ifndef SPACEINVADERS_LINKEDLIST_H
#define SPACEINVADERS_LINKEDLIST_H

typedef void (*freeFunction)(void *);

typedef struct Nodes {
    struct Nodes *next;
    void *data;
} Node;

struct LinkedList {
    Node *head;
    Node *tail;
    int size;
    int elementSize;
    freeFunction freeFn;
};

void createList(struct LinkedList *list, int elementSize, freeFunction fn);
void add(struct LinkedList *list, void *data);
void delete_node(struct LinkedList *list, int index);
void *get(struct LinkedList *list, int index);
int length(struct LinkedList *list);
void list_destroy(struct LinkedList *list);



#endif //SPACEINVADERS_LINKEDLIST_H
