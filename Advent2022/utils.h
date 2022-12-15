#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct vec
    {
        unsigned int capacity;
        unsigned int size;
        void** items;
    }vector;
    int vectorInit(vector* vec);
    int vectorAdd(vector* vec, void* newItem);
    void* vectorRemove(vector* vec, int index);
    int vectorFree(vector* vec, void (*itemfreefunc)(void*));
    void* vectorItemAt(vector* vec, int index);
#ifdef __cplusplus
}
#endif