#ifndef SHIELDCALCULATOR_LIST_H
#define SHIELDCALCULATOR_LIST_H

#define ELEM_SIZE sizeof(void*)
#define SIZE_INCREASE 1024
#define START_SIZE SIZE_INCREASE

struct List {
    int length;
    int start;
    int ending;
    int arrayLength;
    void **array;
};

struct List *newList(int);

void add(struct List *, void *);

void *delete(struct List *, int);

void resize(struct List *, int);

void *get(struct List *, int);

int contains(struct List *, void *);

void printList(struct List *);

void freeList(struct List*);

#endif //SHIELDCALCULATOR_LIST_H
