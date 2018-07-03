#include "linkedlist.h"
#include <string.h>
#include <stdlib.h>

LinkedList_T *ll_set_copy(LinkedList_T *original, void *value, size_t nbytes)
{
    original->value = malloc(nbytes);
    memcpy(original->value, value, nbytes);
    original->dynamically_allocated = 1;
    original->valuec = nbytes;
    return original;
}

LinkedList_T *ll_set_nocopy(LinkedList_T *original, void *value, int dynamically_allocated, size_t valuec)
{
    original->value = value;
    original->dynamically_allocated = dynamically_allocated;
    original->valuec = valuec;
    return original;
}

LinkedList_T *ll_create_empty()
{
    LinkedList_T * l = malloc(sizeof(LinkedList_T));
    l->value = NULL;
    l->next = NULL;
    l->dynamically_allocated = 0;
    l->valuec = 0;
    return l;
}


LinkedList_T *ll_create_filled_copy(void *value, size_t nbytes)
{
    return ll_set_copy(ll_create_empty(), value, nbytes);

}

LinkedList_T *ll_create_filled_nocopy(void *value, int dynamically_allocated, size_t valuec)
{
    return ll_set_nocopy(ll_create_empty(), value, dynamically_allocated, valuec);
}

LinkedList_T *ll_append(LinkedList_T *original, LinkedList_T *toappend)
{
   original->next = toappend; 
   return original->next;
}

LinkedList_T *ll_copy(LinkedList_T *original, size_t limit)
{
    LinkedList_T *copy = ll_create_empty();

    LinkedList_T *copy_current = copy;
    LinkedList_T *original_current = original;

    for(int i = 0; limit == 0 ? 1 : limit < i; ++i)
    {
        copy_current = ll_set_copy(copy_current, original_current->value, original_current->valuec);

        copy_current = ll_append(copy_current, ll_create_empty());

        original_current = original_current->next;

        if(original_current->value == NULL) break;
    }
    return copy;
}

void ll_free(LinkedList_T *tofree, void (*free_function)(void*))
{
    LinkedList_T *temp;
    LinkedList_T *current = tofree;

    while(1) 
    {
        temp = current;

        if(current->dynamically_allocated && free_function != NULL) free_function(current->value);
    
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
    size_t c = 0;
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
