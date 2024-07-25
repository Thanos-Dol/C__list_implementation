#ifndef LIST_H__RESURVIVOR
#define LIST_H__RESURVIVOR



#define DEFAULT_MAXISE __INT64_MAX__



/* ----------------------- listnode_value ------------------------- */


typedef struct {

    double value;

} listnode_value;




listnode_value* listnode_value_init(double value);


void listnode_value_destroy(listnode_value* a_listnode);


void listnode_value_set_value(listnode_value* a_listnode_value, double value);


short listnode_equal(listnode_value* v1, listnode_value* v2);

/* ----------------------- listnode ------------------------- */


typedef struct listnode listnode;

struct listnode {

    listnode_value* value;
    listnode* next;
    listnode* prev;

};




listnode* listnode_init(listnode_value* a_listnode_value, listnode* next, listnode* prev);

void listnode_destroy(listnode* a_listnode);


/* --------------------------- list ------------------------- */


typedef struct {

    listnode* head;
    listnode* tail;
    unsigned long size;
    unsigned long long maxsize;

} list;




list* list_init();

void list_destroy(list* a_list);

unsigned short list_is_empty(list* a_list);


/* -------------- get ----------------- */


unsigned long list_get_size(list* a_list);


listnode_value* list_get_head(list* a_list);


double list_get_head_uncoated(list* a_list);


listnode_value* list_get_tail(list* a_list);


double list_get_tail_uncoated(list* a_list);


listnode_value** list_get_array_copy(list* a_list);


double* list_get_array_copy_uncoated(list* a_list);


/* ------ add -------- */


void list_append(list* a_list, listnode_value* a_listnode_value);


void list_add_front(list* a_list, listnode_value* a_listnode_value);


void list_add(list* a_list, listnode_value* a_listnode_value, long int pos);


/* ------- remove -------- */


void list_remove_head(list* a_list);


void list_remove_tail(list* a_list);


void list_remove_at_pos(list* a_list, long int pos);


void list_clear(list* a_list);


/* ------- find --------*/


/* returns first occurence of the element */
long int list_find(list* a_list, listnode_value* a_listnode_value);


/* ------ print ------- */


void list_print(list* a_list);




#endif /* LIST_H__RESURVIVOR */