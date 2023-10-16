//
// Created by mouha on 22/11/2022.
//

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "pattern_matching.h"


int pm_init(pm_t *pm) {
    pm->newstate++;
    pm->zerostate = (pm_state_t *) malloc(sizeof(pm_state_t));
    if ((pm->zerostate->_transitions = (dbllist_t *) malloc(sizeof(dbllist_t))) == NULL)
        return -1;
    else
        dbllist_init(pm->zerostate->_transitions);
    pm->zerostate->id = 0;
    pm->zerostate->depth = 0;
    if ((pm->zerostate->output = (dbllist_t *) malloc(sizeof(dbllist_t))) == NULL) {
        return -1;
    } else
        dbllist_init(pm->zerostate->output);
    pm->zerostate->fail = pm->zerostate;


    return 0;
}

int pm_addstring(pm_t *pm, unsigned char *Str, _size_t n) {
    if (pm == NULL)
        return -1;
    if (Str == NULL)
        return -1;
    int i = 0;
    pm_state_t *to = NULL;
    pm_state_t *temp = pm->zerostate;


    while (i < n) {
        to = pm_goto_get(temp, Str[i]);
        if (to == NULL)
        {
            to = (pm_state_t *) malloc(sizeof(pm_state_t));
            if (to == NULL)
                return -1;
            to->id = pm->newstate;


            to->output = (dbllist_t *) malloc(sizeof(dbllist_t));
            if (to->output == NULL)
                return -1;
            dbllist_init(to->output);
            to->depth = i + 1;
            to->fail = NULL;
            pm->newstate += 1;
            to->_transitions = (dbllist_t *) malloc(sizeof(dbllist_t));
            if (to->_transitions == NULL)
                return -1;
            dbllist_init(to->_transitions);
            if (pm_goto_set(temp, Str[i], to) == -1)
                return -1;
        }
        temp = to;


        i++;
    }
    if (dbllist_append(temp->output, Str) == -1)
        return -1;
    return 0;

}


int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state) {
    pm_labeled_edge_t *new_edge = (pm_labeled_edge_t *) malloc(sizeof(pm_labeled_edge_t));
    if (new_edge == NULL)
        return -1;
    new_edge->label = symbol;
    new_edge->state = to_state;
    to_state->depth = from_state->depth + 1;
    if (dbllist_append(from_state->_transitions, (void *) new_edge) == -1)
        return -1;
    printf("Allocating state %d\n", new_edge->state->id);
    printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);
    return 0;
}

pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol) {
    if (state->_transitions == NULL)
        return NULL;
    dbllist_node_t *temp = dbllist_head(state->_transitions);


    while (temp != NULL) {
        if (((pm_labeled_edge_t *) (dbllist_data(temp)))->label == symbol) {
            return ((pm_labeled_edge_t *) (dbllist_data(temp)))->state;
        } else {
            temp = dbllist_next(temp);
        }
    }

    return NULL;

}

int dbllist_join_two_Lists(dbllist_t *list1, dbllist_t *list2) {
    if ((list1 == NULL) || (list2 == NULL))
        return -1;
    int i;
    dbllist_node_t *temp = dbllist_head(list2);
    for (i = 0; i < dbllist_size(list2); i++) {
        if (dbllist_append(list1, dbllist_data(temp)) == -1)
            return -1;
        temp = dbllist_next(temp);
    }
    return 0;
}

void *pop(dbllist_t *list) {
    void *data = dbllist_data(dbllist_head(list));
    dbllist_node_t *temp = dbllist_head(list);
    dbllist_head(list) = dbllist_next(temp);
    free(temp);
    dbllist_size(list)--;
    return data;
}

int pm_makeFSM(pm_t *pm) {

    if (pm == NULL)
        return -1;
    int i;
    dbllist_t *list = pm->zerostate->_transitions;
    dbllist_node_t *tempHead = dbllist_head(list);
    dbllist_t *queue = (dbllist_t *) malloc(sizeof(dbllist_t));
    if (queue == NULL)
        return -1;
    dbllist_init(queue);

    for (i = 0; i < dbllist_size(pm->zerostate->_transitions); i++) {

        pm_labeled_edge_t *edge = (pm_labeled_edge_t *) dbllist_data(tempHead);
        if (dbllist_append(queue, edge->state) == -1)
            return -1;
        edge->state->fail = pm->zerostate;
        tempHead = dbllist_next(tempHead);
    }

    while (dbllist_head(queue) != NULL) {

        pm_state_t *state = (pm_state_t *) pop(queue);
        if (state == NULL)
            return -1;
        tempHead = dbllist_head(state->_transitions);
        for (i = 0; i < dbllist_size(state->_transitions); i++) {
            pm_labeled_edge_t *edge = (pm_labeled_edge_t *) dbllist_data(tempHead);
            pm_state_t *fail_state = state->fail;
            pm_state_t *goTo_state = pm_goto_get(fail_state, edge->label);
            while (goTo_state == NULL) {
                if (fail_state->depth == 0)
                {
                    goTo_state = pm->zerostate;
                    break;
                }
                fail_state = fail_state->fail;
                goTo_state = pm_goto_get(fail_state, edge->label);
            }
            edge->state->fail = goTo_state;
            dbllist_join_two_Lists(edge->state->output,
                                   goTo_state->output);
            dbllist_append(queue, edge->state);
            printf("Setting f(%u) = %u\n", edge->state->id, edge->state->fail->id);
            tempHead = dbllist_next(tempHead);
        }

    }
    free(queue);
    return 0;
}

dbllist_t *pm_fsm_search(pm_state_t *state, unsigned char *Str, _size_t n) {
    if (state == NULL || Str == NULL)
        return NULL;
    int i = 0, j;
    pm_state_t *to;
    dbllist_t *Matches = (dbllist_t *) malloc(sizeof(dbllist_t));
    if (Matches == NULL)
        return NULL;
    dbllist_init(Matches);
    while (i < n) {
        to = pm_goto_get(state, Str[i]);
        pm_state_t *fail_state = state;

        while (to == NULL)
        {
            if (fail_state->depth == 0) {

                to = fail_state;
                break;
            }
            fail_state = fail_state->fail;
            if (fail_state == NULL) {
                dbllist_destroy(Matches, DBLLIST_FREE_DATA);
                return NULL;


            }
            to = pm_goto_get(fail_state, Str[i]);
        }
        if (dbllist_head(to->output) != NULL) {
            dbllist_node_t *ptr = dbllist_head(to->output);
            for (j = 0; j < dbllist_size(to->output); j++) {

                pm_match_t *newMatch = (pm_match_t *) malloc(sizeof(pm_match_t));
                if (newMatch == NULL)
                    return NULL;
                newMatch->pattern = dbllist_data(ptr);
                newMatch->start_pos = i - strlen(newMatch->pattern) + 1;
                newMatch->end_pos = i;
                newMatch->fstate = to;
                dbllist_append(Matches, newMatch);
                ptr = dbllist_next(ptr);
            }
        }

        state = to;
        i++;
    }
    return Matches;
}

void destroy_pv(pm_state_t *state) {
    int i = 0;
    dbllist_node_t *node = dbllist_head(state->_transitions);

    while (i < dbllist_size(state->_transitions)) {

        destroy_pv(((pm_labeled_edge_t *) dbllist_data(node))->state);
        node = dbllist_next(node);
        i++;
    }

    dbllist_destroy(state->output, DBLLIST_LEAVE_DATA);

    free(state->output);

    dbllist_destroy(state->_transitions, DBLLIST_FREE_DATA);

    free(state->_transitions);

    free(state);
}

void pm_destroy(pm_t *pm)
{
    if (pm == NULL)
        return;
    destroy_pv(pm->zerostate);

}
