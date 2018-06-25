#ifndef CALC_LINKEDLIST
#define CALC_LINKEDLIST

typedef struct LinkedList LinkedList_T;

struct LinkedList {
    void *value;
    LinkedList_T *next;
};



#endif // CALC_LINKEDLIST
