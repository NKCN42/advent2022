#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

typedef struct position
{
    int x;
    int y;
} pos;
typedef struct poslist
{
    pos** list; 
    int numElements; 
    int capacity; 
}posVector;

void initPosVector(posVector* vec)
{
    vec->capacity = 2; 
    vec->list = (pos**)calloc(2, sizeof(pos*)); 
    vec->numElements = 0; 
}

int addPos(posVector* posList, int x, int y)
{
    int ret = 0; 
    pos** temp; 
    pos* temp2; 
    //if position in list, do nothing
    if (!checkPos(posList, x, y))
    {
        //add to end of list
        if (posList->numElements + 1 > posList->capacity)
        {
            //realloc and go
            temp = realloc(posList->list, sizeof(pos) * posList->capacity * 2); 
            if (temp == NULL)
            {
                ret = 1; 
                goto cleanup; 
            }
            posList->capacity *= 2; 
        }
        temp2 = (pos*)calloc(1, sizeof(pos));
        if (temp2 == NULL)
        {
            ret = 1; 
            goto cleanup; 
        }
        temp2->x = x; 
        temp2->y = y; 
        posList->list[posList->numElements] = temp2; 
        posList->numElements++; 
    }
cleanup: 
    return ret; 
}

int checkPos(posVector* posList, int x, int y)
{
    int ret = 0; 
    for (int i = 0; i < posList->numElements; i++)
    {
        if (posList->list[i]->x == x && posList->list[i]->y == y)
        {
            //found a match
            ret = 1; 
            break; 
        }
    }

    return ret; 
}

int freePosVector(posVector* posList)
{
    int ret = 0; 
    for (int i = 0; i < posList->numElements; i++)
    {
        free(posList->list[i]); 
    }
    free(posList->list); 
    return ret; 
}

int day9()
{
    int ret = 0;
    FILE* part1 = NULL;
    char buf[16]; 
    pos head = { 0 };
    pos tail = { 0 };
    posVector tailPositionList = { 0 };
    int numTailPos = 0;
    char* context; 
    char* direction; 
    int amountToMove; 
    printf("Day 9\n");
    ret = fopen_s(&part1, "day9.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    //Phase 1: Initialize
    head.x = 0;
    head.y = 0;

    tail.x = 0;
    tail.y = 0;

    initPosVector(&tailPositionList); 
    addPos(&tailPositionList, 0, 0); 
    
    //Phase 2: Simulation
    while (fgets(buf, 16, part1))
    {
        //we need a direction from this
        direction = strtok_s(buf, " ", &context); 
        amountToMove = strtol(strtok_s(buf, " ", &context), NULL, 10); 
        switch (direction[0])
        {
        case 'U': 
            for (int i = 0; i < amountToMove; i++)
            {
                head.x += 1; 
                /*
                    get absolute value of:
                        (1) head.x - tail.x 
                        (2) head.y - tail.y 
                    if either is > 1, tail needs to move. 
                    if one is > 1 and the other is nonzero, will need to move diagonally
                    the two directions of the diagonal is based off of two things
                        - the direction of the movement (U in this case)
                        - the relative value of the other direction (in U's case, to the left and right

                */
            }
            break; 
        case 'D': 
            break; 
        case 'L':
            break; 
        case 'R': 
            break; 
        }
    }

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    if (tailPositionList.list != NULL)
    {
        freePosVector(&tailPositionList); 
    }
    return ret;
}