#include "linkedlist.h"
#include <string.h>
#include <stdlib.h>

LinkedList_T *ll_set_copy(LinkedList_T *original, void *value, size_t nbytes)
{
    original->value = malloc(nbytes);
    memcpy(original->value, value, nbytes);
    original->dynamically_allocated = 1;
    return original;
}

LinkedList_T *ll_set_nocopy(LinkedList_T *original, void *value, int dynamically_allocated)
{
    original->value = value;
    original->dynamically_allocated = dynamically_allocated;
    return original;
}

LinkedList_T *ll_create_empty()
{
    LinkedList_T * l = malloc(sizeof(LinkedList_T));
    l->value = NULL;
    l->next = NULL;
    l->dynamically_allocated = 0;
    return l;
}


LinkedList_T *ll_create_filled_copy(void *value, size_t nbytes)
{
    return ll_set_copy(ll_create_empty(), value, nbytes);

}

LinkedList_T *ll_create_filled_nocopy(void *value, int dynamically_allocated)
{
    return ll_set_nocopy(ll_create_empty(), value, dynamically_allocated);
}

LinkedList_T *ll_append(LinkedList_T *original, LinkedList_T *toappend)
{
   original->next = toappend; 
   return original->next;
}

void ll_free(LinkedList_T *tofree)
{
    LinkedList_T *temp;
    LinkedList_T *current = tofree;

    while(1) 
    {
        temp = current;

        if(current->dynamically_allocated) free(current->value);
    
        if(current->value == NULL)
        {
            free(current);
            break;
        }
        current = current->next;
        free(temp);
    }
}

size_t ll_size(LinkedList_T *l)
{
    LinkedList_T *current = l;
    size_t c = 1;
    while(current->value != NULL)
    {
        c++;
        current = current->next;
    }
    return c;
}

void **ll_to_array(LinkedList_T *original)
{
    void **result = malloc(0);
    LinkedList_T *current = original;

    size_t nodec = 0;
    while(1)
    {
        result = realloc(result, sizeof(void*) * ++nodec);
        result[nodec-1] = current->value; /* should automagically NULL terminate */

        if(current->value == NULL) break;
        current = current->next;
    }
    return result;
}

/*
LinkedList_T *fill = append_llnode(append_llnode(create_emptyll(), create_filledll_nocopy("abcdef", 0)), create_filledll_nocopy(malloc(8), 1));

*/
