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


void list_destroy(list* a_list) {

    listnode* curr;
    listnode* tmp;

    curr = a_list -> head;

    while (curr)
    {
        tmp = curr -> next;
        listnode_destroy(curr);
        curr = tmp;
    }
    free(a_list);

    return;
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


listnode_value** list_get_array_copy(list* a_list) {
    
    if (!(a_list -> size))
    {
        return NULL;
    }

    listnode_value** result;
    result = (listnode_value**) calloc(a_list -> size, sizeof(listnode*));

    int i;
    listnode* tmp;
    listnode_value* listnode_value_copy;

    i = 0;
    tmp = a_list -> head;

    while (tmp)
    {
        listnode_value_copy = (listnode_value*) malloc(sizeof(listnode_value));
        listnode_value_set_value(listnode_value_copy, tmp -> value -> value);
        result[i] = listnode_value_copy;
        i += 1;
        tmp = tmp -> next;
    }

    return result;
}


double* list_get_array_copy_uncoated(list* a_list) {

    if (!(a_list -> size))
    {
        return NULL;
    }

    double* result;
    result = (double*) calloc(a_list -> size, sizeof(double));

    int i;
    listnode* tmp;

    i = 0;
    tmp = a_list -> head;

    while (tmp)
    {
        result[i] = tmp -> value -> value;
        i += 1;
        tmp = tmp -> next;
    }
    
    return result;
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


void list_add(list* a_list, listnode_value* a_listnode_value, long int pos) {
    
    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }


// printf("%d %d %d %d\n", pos, a_list -> size, -(a_list -> size), (pos) < (-(a_list -> size)));  //!!!!!!!!!!!! this the reason we love C!
    

    if ((pos < (-((long int)a_list -> size))) || (pos > ((long int)a_list -> size)))  //! due to unsigned long conversion to long int there will be precision errors
    {
        return;
    }

    if (pos < 0)
    {
        pos = pos + (long int)(a_list -> size);
    }

    if (!(a_list -> size))
    {
        listnode* new_node;
        new_node = listnode_init(a_listnode_value, NULL, NULL);

        a_list -> head = new_node;
        a_list -> tail = new_node;

        a_list -> size += 1;
        return;
    }

    if (!(pos))
    {
        listnode* new_node;
        new_node = listnode_init(a_listnode_value, a_list -> head, NULL);

        new_node -> next -> prev = new_node;

        a_list -> head = new_node;

        a_list -> size += 1;
        return;
    }

    if (pos == a_list -> size)
    {
        listnode* new_node;
        new_node = listnode_init(a_listnode_value, NULL, a_list -> tail);

        new_node -> prev -> next = new_node;

        a_list -> tail = new_node;

        a_list -> size += 1;
        return;
    }

    if (pos <= ((a_list -> size - 1) / 2))
    {
        listnode* tmp;
        tmp = a_list -> head;

        int i;
        for (i = 0; i < pos - 1; ++i)
        {
            tmp = tmp -> next;
        }

        listnode* new_node;
        new_node = listnode_init(a_listnode_value, tmp -> next, tmp);

        tmp -> next = new_node;
        new_node -> next -> prev = new_node;
    }
    else
    {
        listnode* tmp;
        tmp = a_list -> tail;

        int i;
        for (i = 0; i < (a_list -> size - pos - 1); ++i)
        {
            tmp = tmp -> prev;
        }

        listnode* new_node;
        new_node = listnode_init(a_listnode_value, tmp, tmp -> prev);

        tmp -> prev = new_node;
        new_node -> prev -> next = new_node;
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


void list_remove_at_pos(list* a_list, long int pos) {

    if (!(a_list -> size))
    {
        return;
    }

    if (pos < (-((long int)a_list -> size)) || pos > (((long int)a_list -> size) - 1))
    {
        return;
    }

    if (pos < 0)
    {
        pos = pos + (long int)a_list -> size;
    }

    if (a_list -> size == 1)
    {
        listnode_destroy(a_list -> head);
        a_list -> head = NULL;
        a_list -> tail = NULL;
        a_list -> size = 0;
        return;
    }

    if (!(pos))
    {
        listnode* tmp;
        tmp = a_list -> head;

        a_list -> head = tmp -> next;

        tmp -> next -> prev = NULL;

        listnode_destroy(tmp);

        a_list -> size -= 1;

        return;
    }

    if (pos == a_list -> size - 1)
    {
        listnode* tmp;
        tmp = a_list -> tail;

        a_list -> tail = tmp -> prev;

        tmp -> prev -> next = NULL;

        listnode_destroy(tmp);

        a_list -> size -= 1;

        return;
    }

    if (pos <= (((long int)a_list -> size - 1) / 2))
    {
        listnode* tmp;
        tmp = a_list -> head;

        int i;

        for (i = 0; i < pos - 1; ++i)
        {
            tmp = tmp -> next;
        }

        listnode* listnode_to_remove;
        listnode_to_remove = tmp -> next;

        tmp -> next = listnode_to_remove -> next;
        listnode_to_remove -> next -> prev = tmp;

        listnode_destroy(listnode_to_remove);
    }
    else 
    {
        listnode* tmp;
        tmp = a_list -> tail;

        int i;

        for (i = 0; i < ((long int)a_list -> size - pos - 2); ++i)
        {
            tmp = tmp -> prev;
        }

        listnode* listnode_to_remove;
        listnode_to_remove = tmp -> prev;

        tmp -> prev = listnode_to_remove -> prev;
        listnode_to_remove -> prev -> next = tmp;

        listnode_destroy(listnode_to_remove);
    }

    a_list -> size -= 1;
}


void list_clear(list* a_list) {
    
    if (!(a_list -> size))
    {
        return;
    }

    listnode* tmp_1;
    listnode* tmp_2;

    tmp_1 = a_list -> head;
    tmp_2 = NULL;

    while (tmp_1)
    {
        tmp_2 = tmp_1 -> next;
        listnode_destroy(tmp_1);
        tmp_1 = tmp_2;
    }

    a_list -> head = NULL;
    a_list -> tail = NULL;
    a_list -> size = 0;
    return;
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