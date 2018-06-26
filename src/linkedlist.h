#ifndef CALC_LINKEDLIST
#define CALC_LINKEDLIST

#include <stddef.h>

typedef struct LinkedList LinkedList_T;

struct LinkedList {
    void *value;
    LinkedList_T *next;
    int dynamically_allocated;
};


LinkedList_T *ll_set_copy(LinkedList_T *original, void *value, size_t nbytes);
LinkedList_T *ll_set_nocopy(LinkedList_T *original, void *value, int dynamically_allocated);

LinkedList_T *ll_create_empty();
LinkedList_T *ll_create_filled_copy(void *value, size_t nbytes);
LinkedList_T *ll_create_filled_nocopy(void *value, int is_dynamically_allocated);
LinkedList_T *ll_append(LinkedList_T *original, LinkedList_T *toappend);



void ll_free(LinkedList_T *tofree);
size_t ll_size(LinkedList_T *l);
#endif // CALC_LINKEDLIST
