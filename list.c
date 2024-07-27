#include <stdio.h>
#include <malloc.h>
#include "list.h"



// ! TODO make sure there are no memory leaks!



/* ----------------------- listnode ------------------------- */




listnode* listnode_init(void* data, listnode* next, listnode* prev) {

    listnode* new_node;
    new_node = (listnode*) malloc(sizeof(listnode));

    new_node -> data = data;
    new_node -> next = next;
    new_node -> prev = prev;

    return new_node;
}


void listnode_destroy(listnode* a_listnode, void (*data_destroyer)(void*))
{
    data_destroyer(a_listnode -> data);
    free(a_listnode);
}


/* --------------------------- list ------------------------- */




list* list_init(
    void* (*data_copy_callback)(void*),
    void (*data_destroyer_callback)(void*),
    int (*data_equal_callback)(void* data1, void* data2),
    void (*data_print_callback)(void* data)
) {

    list* new_list;

    new_list = (list*) malloc(sizeof(list));

    new_list -> head = NULL;
    new_list -> tail = NULL;
    new_list -> size = 0;
    new_list -> maxsize = DEFAULT_MAXISE;

    new_list -> data_copy = data_copy_callback;
    new_list -> data_destroyer = data_destroyer_callback;
    new_list -> data_equal = data_equal_callback;
    new_list -> data_print = data_print_callback;

    return new_list;
}


void list_destroy(list* a_list) {

    listnode* curr;
    listnode* tmp;

    curr = a_list -> head;

    while (curr)
    {
        tmp = curr -> next;
        listnode_destroy(curr, a_list -> data_destroyer);
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


void* list_get_head(list* a_list) {

    if (!(a_list -> size))
    {
        return NULL;
    }

    return a_list -> data_copy(a_list -> head -> data);
}


void* list_get_tail(list* a_list) {

    if (!(a_list -> size))
    {
        return NULL;
    }

    return a_list -> data_copy(a_list -> tail -> data);
}


void** list_get_array_copy(list* a_list) {
    
    if (!(a_list -> size))
    {
        return NULL;
    }

    void** result;
    result = (void**) calloc(a_list -> size, sizeof(void*));

    int i;
    listnode* tmp;
    void* listnode_data_copy;

    i = 0;
    tmp = a_list -> head;

    while (tmp)
    {
        listnode_data_copy = a_list -> data_copy(tmp -> data);
        result[i] = listnode_data_copy;
        i += 1;
        tmp = tmp -> next;
    }

    return result;
}


/* ------ add -------- */


void list_append(list* a_list, void* incoming_data) {

    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }

    void* incoming_data_copy = a_list -> data_copy(incoming_data);
    
    // ! TODO check if listnode_init first creates a copy of void* data and stores copy so creating another copy is redundant here
    listnode* tmp = listnode_init(incoming_data_copy, NULL, NULL);

    if (!(a_list -> size))
    {
        a_list -> tail = tmp;
        a_list -> head = tmp;
        tmp -> next = NULL;  // redundant but helps with readability
        tmp -> prev = NULL;  // redundant but helps with readability
    }
    else
    {
        tmp -> prev = a_list -> tail;
        tmp -> next = NULL;  // redundant but helps with readability
        a_list -> tail -> next = tmp;
        a_list -> tail = tmp;
    }

    a_list -> size += 1;

    return;
}


void list_add_front(list* a_list, void* incoming_data) {
    
    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }

    void* incoming_data_copy = a_list -> data_copy(incoming_data);

    // ! TODO check if listnode_init first creates a copy of void* data and stores copy so creating another copy is redundant here
    listnode* tmp = listnode_init(incoming_data_copy, NULL, NULL);

    if (!(a_list -> size))
    {
        a_list -> head = tmp;
        a_list -> tail = tmp;
        tmp -> next = NULL;  // redundant but helps with readability
        tmp -> prev = NULL;  // redundant but helps with readability
    }
    else
    {
        tmp -> next = a_list -> head;
        tmp -> prev = NULL;  // redundant but helps with readability
        a_list -> head -> prev = tmp;
        a_list -> head = tmp;
    }

    a_list -> size += 1;

    return;
}


void list_add(list* a_list, void* incoming_data, long int pos) {
    
    if (a_list -> size == a_list -> maxsize)
    {
        return;
    }    

    if ((pos < (-((long int)a_list -> size))) || (pos > ((long int)a_list -> size)))  // ! TODO due to unsigned long conversion to long int there will be precision errors, scecify boundaries for accepted values
    {
        return;
    }

    if (pos < 0)
    {
        pos = pos + (long int)(a_list -> size);
    }

    void* incoming_data_copy = a_list -> data_copy(incoming_data);

    if (!(a_list -> size))
    {
        listnode* new_node;
        new_node = listnode_init(incoming_data_copy, NULL, NULL);

        a_list -> head = new_node;
        a_list -> tail = new_node;

        a_list -> size += 1;
        return;
    }

    if (!(pos))
    {
        listnode* new_node;
        new_node = listnode_init(incoming_data_copy, a_list -> head, NULL);

        new_node -> next -> prev = new_node;

        a_list -> head = new_node;

        a_list -> size += 1;
        return;
    }

    if (pos == a_list -> size)
    {
        listnode* new_node;
        new_node = listnode_init(incoming_data_copy, NULL, a_list -> tail);

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
        new_node = listnode_init(incoming_data_copy, tmp -> next, tmp);

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
        new_node = listnode_init(incoming_data_copy, tmp, tmp -> prev);

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

    listnode_destroy(tmp, a_list -> data_destroyer);

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

    listnode_destroy(tmp, a_list -> data_destroyer);

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
        listnode_destroy(a_list -> head, a_list -> data_destroyer);
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

        listnode_destroy(tmp, a_list -> data_destroyer);

        a_list -> size -= 1;

        return;
    }

    if (pos == a_list -> size - 1)
    {
        listnode* tmp;
        tmp = a_list -> tail;

        a_list -> tail = tmp -> prev;

        tmp -> prev -> next = NULL;

        listnode_destroy(tmp, a_list -> data_destroyer);

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

        listnode_destroy(listnode_to_remove, a_list -> data_destroyer);
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

        listnode_destroy(listnode_to_remove, a_list -> data_destroyer);
    }

    a_list -> size -= 1;
}


void list_clear(list* a_list)
{
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
        listnode_destroy(tmp_1, a_list -> data_destroyer);
        tmp_1 = tmp_2;
    }

    a_list -> head = NULL;
    a_list -> tail = NULL;
    a_list -> size = 0;
    return;
}


/* ------- find --------*/


/* returns first occurence of the element */
long int list_find(list* a_list, void* given_data) {

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
        if (a_list -> data_equal(tmp -> data, given_data)) 
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
        a_list -> data_print(tmp -> data);
        tmp = tmp -> next;
    }
    printf("\n");
}