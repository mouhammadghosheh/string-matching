#include "slist.h"
#include <stdio.h>
#include <stdlib.h>


void dbllist_init(dbllist_t *list) {

    dbllist_head(list) = NULL;
    dbllist_tail(list) = NULL;
    dbllist_size(list) = 0;
}

int dbllist_append(dbllist_t *list, void *data) {
    dbllist_node_t *new_node;
    new_node = (dbllist_node_t *) malloc(sizeof(dbllist_node_t));
    if (new_node == NULL)
        return -1;
    else {
        dbllist_data(new_node) = data;
        dbllist_next(new_node) = NULL;
        dbllist_prev(new_node) = NULL;


        if (list->size == 0) {
            dbllist_head(list) = dbllist_tail(list) = new_node;
            dbllist_size(list)++;
            return 0;
        }
        dbllist_next(dbllist_tail(list)) = new_node;
        dbllist_tail(list) = new_node;
        dbllist_size(list)++;
        return 0;
    }


}

int dbllist_remove(dbllist_t *to, dbllist_node_t *pointer, dbllist_destroy_t dealloc) {


    if (dbllist_head(to) == NULL || pointer == NULL) {

        return -1;
    } else if (dbllist_head(to) == pointer) {
        dbllist_head(to) = dbllist_next(pointer);

    } else if (dbllist_tail(to) == pointer) {
        dbllist_tail(to) = dbllist_prev(dbllist_tail(to));
    } else if (dbllist_next(pointer) != NULL) {
        dbllist_prev(dbllist_next(pointer)) = dbllist_prev(pointer);
    } else if (dbllist_prev(pointer) != NULL) {
        dbllist_next(dbllist_prev(pointer)) = dbllist_next(pointer);
    }

    if (dealloc == DBLLIST_LEAVE_DATA) {
        free(pointer);

    } else {
        free(dbllist_data(pointer));
        free(pointer);
    }


    dbllist_size(to)--;
    return 0;
}

int dbllist_prepend(dbllist_t *list, void *data) {
    dbllist_node_t *new_node;
    new_node = (dbllist_node_t *) malloc(sizeof(dbllist_node_t));
    if (new_node == NULL)
        return -1;
    else {
        dbllist_data(new_node) = data;
        dbllist_next(new_node) = NULL;
        dbllist_prev(new_node) = NULL;

        if (dbllist_size(list) == 0) {
            dbllist_head(list) = new_node;
            dbllist_size(list)++;
            return 0;
        } else {
            dbllist_next(new_node) = dbllist_head(list);
            dbllist_head(list) = new_node;
            dbllist_size(list)++;
            return 0;
        }


    }
}

void dbllist_destroy(dbllist_t *List, dbllist_destroy_t dealloc) {
    if (List == NULL) {
        return;
    }
    while (dbllist_head(List) != NULL) {
        dbllist_remove(List, dbllist_head(List), dealloc);
    }
}

