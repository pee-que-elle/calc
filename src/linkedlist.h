#ifndef CALC_LINKEDLIST
#define CALC_LINKEDLIST

#include <stddef.h>

typedef struct LinkedList LinkedList_T;

struct LinkedList {
    void *value;
    LinkedList_T *next;
    int is_dynamically_allocated;
};


LinkedList_T *create_emptyll();
LinkedList_T *create_filledll_copy(void *value, size_t nbytes);
LinkedList_T *create_filledll_nocopy(void *value, int is_dynamically_allocated);
LinkedList_T *append_llnode(LinkedList_T *original, LinkedList_T *toappend);
void free_ll(LinkedList_T *tofree);
size_t get_llsize(LinkedList_T *l);
#endif // CALC_LINKEDLIST
