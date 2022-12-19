#include <stdio.h>
#include <Windows.h>
#include "utils.h"


void vectorInit(vector* vec)
{
    vec->items = (void**)calloc(2, sizeof(void*));
    vec->capacity = 2;
    vec->size = 0;
}


int vectorAdd(vector* vec, void* newItem)
{
    void** temp = NULL;
    int ret = 0;
    if (vec->size == vec->capacity)
    {
        //realloc
        temp = realloc(vec->items, vec->capacity * 2 * sizeof(void*));
        if (temp == NULL)
        {
            ret = 1;
            goto cleanup;
        }
        vec->items = temp;
        vec->capacity *= 2;
    }
    vec->items[vec->size] = newItem;
    vec->size++;
cleanup:
    return ret;
}

void* vectorRemove(vector* vec, int index)
{
    void* ret = NULL;
    if (vec == NULL || vec->size == 0 || index >= vec->size)
    {
        goto cleanup;
    }
    ret = vec->items[index];
    //move everything else down
    for (int i = index; i < vec->size - 1; i++)
    {
        vec->items[i] = vec->items[i + 1];
    }
    //set last element to NULL (whatever's there should still be preserved, though)
    vec->items[vec->size - 1] = NULL;
    vec->size--;

cleanup:
    return ret;
}

int vectorFree(vector* vec, void (*itemfreefunc)(void*))
{
    if (itemfreefunc != NULL)
    {
        //use this function to free all elements of vec->items
        for (int i = 0; i < vec->size; i++)
        {
            itemfreefunc(vec->items[i]);
        }
    }
    //now free items
    free(vec->items);
    return 0;
}

void* vectorItemAt(vector* vec, int index)
{
    void* ret = NULL;
    if (vec != NULL && index < vec->size)
    {
        ret = vec->items[index];
    }
    return ret;
}