#include "linkedlist.h"
#include <string.h>
#include <stdlib.h>

LinkedList_T *create_emptyll()
{
    LinkedList_T * l = malloc(sizeof(LinkedList_T));
    l->value = NULL;
    l->next = NULL;
    l->is_dynamically_allocated = 0;
    return l;
}


LinkedList_T *create_filledll_copy(void *value, size_t nbytes)
{
    LinkedList_T *l = create_emptyll();
    l->value = malloc(nbytes);
    l->is_dynamically_allocated = 1;
    memcpy(l->value, value, nbytes);
    return l;
}

LinkedList_T *create_filledll_nocopy(void *value, int is_dynamically_allocated)
{
    LinkedList_T *l = create_emptyll();
    l->value = value;
    l->is_dynamically_allocated = is_dynamically_allocated;
    return l;
}

LinkedList_T *append_llnode(LinkedList_T *original, LinkedList_T *toappend)
{
   original->next = toappend; 
   return original->next;
}

void free_ll(LinkedList_T *tofree)
{
    LinkedList_T *temp;
    LinkedList_T *current = tofree;

    while(1) 
    {
        temp = current;

        if(current->is_dynamically_allocated) free(current->value);
    
        if(current->next == NULL)
        {
            free(current);
            break;
        }
        current = current->next;
        free(temp);
    }
}

/*
LinkedList_T *fill = append_llnode(append_llnode(create_emptyll(), create_filledll_nocopy("abcdef", 0)), create_filledll_nocopy(malloc(8), 1));

*/
