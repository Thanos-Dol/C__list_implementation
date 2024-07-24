#include <stdio.h>
#include <malloc.h>
#include "list.h"


// ! TODO make sure there are no memory leaks!


// TODO add documentation here
/* ----------------------- listnode_value ------------------------- */




listnode_value* listnode_value_init(double value) {

    listnode_value* new_listnode_value;
    new_listnode_value = (listnode_value*) malloc(sizeof(listnode_value));

    new_listnode_value -> value = value;

    return new_listnode_value;
}


void listnode_value_destroy(listnode_value* a_listnode_value) {
    free(a_listnode_value);
}


void listnode_value_set_value(listnode_value* a_listnode_value, double value) {
    a_listnode_value -> value = value;
}


short listnode_equal(listnode_value* v1, listnode_value* v2) {
    return v1 -> value == v2 -> value;
}

/* ----------------------- listnode ------------------------- */




listnode* listnode_init(listnode_value* a_listnode_value, listnode* next, listnode* prev) {

    listnode* new_node;
    new_node = (listnode*) malloc(sizeof(listnode));

    new_node -> value = listnode_value_init(a_listnode_value -> value);
    new_node -> next = next;
    new_node -> prev = prev;

    return new_node;
}

void listnode_destroy(listnode* a_listnode)
{
    free(a_listnode -> value);
    free(a_listnode);
}


/* --------------------------- list ------------------------- */




list* list_init() {

    list* new_list;

    new_list = (list*) malloc(sizeof(list));

    new_list -> head = NULL;
    new_list -> tail = NULL;
    new_list -> size = 0;
    new_list -> maxsize = DEFAULT_MAXISE;

    return new_list;
}


unsigned short list_is_empty(list* a_list) {

    return !(a_list -> size);
}


/* -------------- get ----------------- */


unsigned long list_get_size(list* a_list) {
    return a_list -> size;
}


listnode_value* list_get_head(list* a_list) {

    if (!(a_list -> size))
    {
        return NULL;
    }

    return a_list -> head -> value;
}


double list_get_head_uncoated(list* a_list) {

    if (!(a_list -> size))
    {
        return 0.0;  // TODO : change this to maximum or minimum value of double (or sth that you will use to represent out of bounds value)
    }
    return a_list -> head -> value -> value;
}


listnode_value* list_get_tail(list* a_list) {

    if (!(a_list -> size))
    {
        return NULL;
    }

    return a_list -> tail -> value;
}


double list_get_tail_uncoated(list* a_list) {

    if (!(a_list -> size))
    {
        return 0.0;  // TODO : change this to maximum or minimum value of double (or sth that you will use to represent out of bounds value)
    }
    return a_list -> tail -> value -> value;
}


/* ------ add -------- */


void list_append(list* a_list, listnode_value* a_listnode_value) {

    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }

    listnode_value* value_copy = (listnode_value*) malloc(sizeof(listnode_value));
    listnode_value_set_value(value_copy, a_listnode_value -> value);
    
    listnode* tmp = (listnode*) malloc(sizeof(listnode));
    tmp -> value = value_copy;

    if (!(a_list -> size))
    {
        a_list -> tail = tmp;
        a_list -> head = tmp;
        tmp -> next = NULL;
        tmp -> prev = NULL;
    }
    else
    {
        tmp -> prev = a_list -> tail;
        tmp -> next = NULL;
        a_list -> tail -> next = tmp;
        a_list -> tail = tmp;
    }

    a_list -> size += 1;

    return;
}


void list_add_front(list* a_list, listnode_value* a_listnode_value) {
    
    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }

    listnode_value* value_copy = (listnode_value*) malloc(sizeof(listnode_value));
    listnode_value_set_value(value_copy, a_listnode_value -> value);

    listnode* tmp = (listnode*) malloc(sizeof(listnode));
    tmp -> value = value_copy;

    if (!(a_list -> size))
    {
        a_list -> head = tmp;
        a_list -> tail = tmp;
        tmp -> next = NULL;
        tmp -> prev = NULL;
    }
    else
    {
        tmp -> next = a_list -> head;
        a_list -> head -> prev = tmp;
        a_list -> head = tmp;
    }

    a_list -> size += 1;

    return;
}


/* ------- remove -------- */


void list_remove_head(list* a_list) {

    if (!(a_list -> size))
    {
        return;
    }

    listnode* tmp;
    tmp = a_list -> head;

    a_list -> head = tmp -> next;

    if (a_list -> size > 1)
    {
        tmp -> next -> prev = NULL;
    }
    else
    {
        a_list -> tail = NULL;
    }

    listnode_destroy(tmp);

    a_list -> size -= 1;
}


void list_remove_tail(list* a_list) {

    if (!(a_list -> size))
    {
        return;
    }

    listnode* tmp;
    tmp = a_list -> tail;

    a_list -> tail = tmp -> prev;

    if (a_list -> size > 1)
    {
        tmp -> prev -> next = NULL;
    }
    else
    {
        a_list -> head = NULL;   
    }

    listnode_destroy(tmp);

    a_list -> size -= 1;
}


/* ------- find --------*/


/* returns first occurence of the element */
long int list_find(list* a_list, listnode_value* a_listnode_value) {

    if (!(a_list -> size))
    {
        return -1;  // TODO : see if you change return value to sth else when element is not found in the list, figure at what value conventions you are gonna use
    }

    int i;
    listnode* tmp;

    i = 0;
    tmp = a_list -> head;

    while (tmp)
    {
        if (listnode_equal(tmp -> value, a_listnode_value)) 
        {
            return i;
        }

        i += 1;
        tmp = tmp -> next;
    }

    return -1;
}


/* ------ print ------- */


void list_print(list* a_list) {

    listnode* tmp;
    tmp = a_list -> head;
    while (tmp)
    {
        printf("%lf ", tmp -> value -> value);
        tmp = tmp -> next;
    }
    printf("\n");
}