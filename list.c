#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct List *newList(int initial) {
    struct List *list = malloc(sizeof(struct List));
    list->arrayLength = initial;
    list->array = malloc(ELEM_SIZE * initial);
    list->length = 0;
    list->ending = 0;
    list->start = 0;
    return list;
}

void resize(struct List *list, int newSize) {
    if (!list || list->arrayLength >= newSize) {
        return;
    }
    list->array = realloc(list->array, ELEM_SIZE * newSize);
    int index = newSize - 1;
    for (int i = list->arrayLength; i > list->ending; i--) {
        list->array[index--] = list->array[i];
    }
    list->arrayLength = newSize;
}

void add(struct List *list, void *toInsert) {
    if (!list || !toInsert) {
        return;
    }

    if (list->arrayLength == list->length) {
        resize(list, list->arrayLength + SIZE_INCREASE);
    }
    if (list->length >= 1) {
        list->ending = (list->ending + 1) % list->arrayLength;
    }
    list->array[list->ending] = toInsert;
    list->length++;

}

void *get(struct List *list, int index) {
    if (index > list->length) {
        return NULL;
    }
    int sz = list->arrayLength;
    int actualIndex = (list->start + index) % sz;
    return list->array[actualIndex];
}

void *delete(struct List *list, int index) {
    if (index > list->length) {
        return NULL;
    }
    struct ShieldBooster *deleted = get(list, index);
    int sz = list->arrayLength;
    int actualIndex = (list->start + index) % sz;
    for (int i = actualIndex - 1; i > list->start; i--) {
        list->array[i] = list->array[i - 1];
    }
    list->length--;
    return deleted;
}

int contains(struct List *list, void *object) {
    for (int i = 0; i < list->length; i++) {
        if (get(list, i) == object) {
            return 1;
        }
    }
    return 0;
}

void printList(struct List *list) {
    for (int i = 0; i < list->length; i++) {
        void *booster = get(list, i);
        printf("Entry #%d, %p\n", i, booster);
    }
}

void freeList(struct List *list) {
    free(list->array);
    free(list);
}

