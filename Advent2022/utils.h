#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#define VEC_INIT_CAPACITY 4
    typedef struct vec
    {
        void** items;
        unsigned int capacity;
        unsigned int size;
    }vector;
    void vectorInit(vector* vec);
    int vectorAdd(vector* vec, void* newItem);
    void* vectorRemove(vector* vec, int index);
    int vectorFree(vector* vec, void (*itemfreefunc)(void*));
    void* vectorItemAt(vector* vec, int index);
#ifdef __cplusplus
}
#endif