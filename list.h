#ifndef LIST_H__RESURVIVOR
#define LIST_H__RESURVIVOR



#define DEFAULT_MAXISE __INT64_MAX__



/* ----------------------- listnode ------------------------- */


typedef struct listnode listnode;

struct listnode {

    void* data;
    listnode* next;
    listnode* prev;

};




listnode* listnode_init(void* data, listnode* next, listnode* prev);

void listnode_destroy(listnode* a_listnode, void (*data_destroyer)(void*));


/* ----------------------- listmaker ------------------------- */


typedef struct {

    void* (*data_copy)(void* data);
    void (*data_destroyer)(void* data);
    int (*data_equal)(void* data1, void* data2);
    void (*data_print)(void* data);

} listmaker;




listmaker* listmaker_init(
    void* (*data_copy)(void* data),
    void (*data_destroyer)(void* data),
    int (*data_equal)(void* data1, void* data2),
    void (*data_print)(void* data)
);


/* --------------------------- list ------------------------- */


typedef struct {

    listnode* head;
    listnode* tail;
    unsigned long size;
    unsigned long long maxsize;

    void* (*data_copy)(void* data);
    void (*data_destroyer)(void* data);
    int (*data_equal)(void* data1, void* data2);
    void (*data_print)(void* data);

} list;




list* list_init(
    void* (*data_copy)(void* data),
    void (*data_destroyer)(void* data),
    int (*data_equal)(void* data1, void* data2),
    void (*data_print)(void* data)
);


list* list_init_listmaker(listmaker* a_listmaker);


void list_destroy(list* a_list);


unsigned short list_is_empty(list* a_list);


/* -------------- get ----------------- */


unsigned long list_get_size(list* a_list);


void* list_get_head(list* a_list);


void* list_get_tail(list* a_list);


void** list_get_array_copy(list* a_list);


double* list_get_array_copy_uncoated(list* a_list);


/* ------ add -------- */


void list_append(list* a_list, void* data);


void list_add_front(list* a_list, void* data);


void list_add(list* a_list, void* data, long int pos);


/* ------- remove -------- */


void list_remove_head(list* a_list);


void list_remove_tail(list* a_list);


void list_remove_at_pos(list* a_list, long int pos);


void list_clear(list* a_list);


/* ------- find --------*/


/* returns first occurence of the element */
long int list_find(list* a_list, void* data);


/* ------ print ------- */


void list_print(list* a_list);




#endif /* LIST_H__RESURVIVOR */