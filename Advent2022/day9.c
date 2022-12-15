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
            posList->list = temp;
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

int getNewTailPosition(pos* head, pos* tail)
{
    pos newTailPos = { 0 };
    newTailPos.x = tail->x;
    newTailPos.y = tail->y;
    int xDiff = 0;
    int yDiff = 0;

    xDiff = head->x - tail->x;
    yDiff = head->y - tail->y;
    //printf("Head: (%d, %d) Tail (%d, %d)\n", head->x, head->y, tail->x, tail->y);
    //printf("Diffs: X: %d Y: %d\n", xDiff, yDiff);
    if (xDiff > 1 || xDiff < -1)
    {
        //adjust x to go towards 0
        if (xDiff > 1)
        {
            newTailPos.x++;
        }
        else
        {
            newTailPos.x--;
        }
        if (yDiff >= 1 || yDiff <= -1)
        {
            //we need to compensate for both directions
            //and honestly, direction shouldn't matter much since both values should attempt to go towards 0
            if (yDiff >= 1)
            {
                newTailPos.y++;
            }
            else
            {
                newTailPos.y--;
            }
        }
    }
    else if (yDiff > 1 || yDiff < -1)
    {
        if (yDiff > 1)
        {
            newTailPos.y++;
        }
        else
        {
            newTailPos.y--;
        }
        if (xDiff >= 1 || xDiff <= -1)
        {
            //we need to compensate for both directions
            //and honestly, direction shouldn't matter much since both values should attempt to go towards 0
            if (xDiff >= 1)
            {
                newTailPos.x++;
            }
            else
            {
                newTailPos.x--;
            }
        }
    }
    tail->x = newTailPos.x;
    tail->y = newTailPos.y;
    //printf("New Tail: (%d, %d)\n", tail->x, tail->y);
    return 0;

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
    int xDiff = 0;
    int yDiff = 0;
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
        amountToMove = strtol(strtok_s(NULL, " ", &context), NULL, 10);
        //printf("%s %d\n", direction, amountToMove);
        for (int i = 0; i < amountToMove; i++)
        {
            switch (direction[0])
            {
            case 'U':
                head.y++;
                break;
            case 'D':
                head.y--;
                break;
            case 'L':
                head.x--;
                break;
            case 'R':
                head.x++;
                break;
            }
            getNewTailPosition(&head, &tail);
            //if tail is not currently in list, add it
            if (!checkPos(&tailPositionList, tail.x, tail.y))
            {
                addPos(&tailPositionList, tail.x, tail.y);
            }
        }

    }

    //score
    printf("    Part 1 Score: %d\n", tailPositionList.numElements);

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

int day9part2()

{
    int ret = 0;
    FILE* part1 = NULL;
    char buf[16];
    pos knots[10];
    posVector tailPositionList = { 0 };
    int numTailPos = 0;
    char* context;
    char* direction;
    int amountToMove;
    int xDiff = 0;
    int yDiff = 0;
    printf("Day 9\n");
    ret = fopen_s(&part1, "day9.txt", "r");
    if (ret != 0)
    {
        printf("     Part 2: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 2: Open successful\n");

    //Phase 1: Initialize
    for (int i = 0; i < 10; i++)
    {
        knots[i].x = 0;
        knots[i].y = 0;
    }

    initPosVector(&tailPositionList);
    addPos(&tailPositionList, 0, 0);

    //Phase 2: Simulation
    while (fgets(buf, 16, part1))
    {
        //we need a direction from this
        direction = strtok_s(buf, " ", &context);
        amountToMove = strtol(strtok_s(NULL, " ", &context), NULL, 10);
        //printf("%s %d\n", direction, amountToMove);
        for (int i = 0; i < amountToMove; i++)
        {
            switch (direction[0])
            {
            case 'U':
                knots[0].y++;
                break;
            case 'D':
                knots[0].y--;
                break;
            case 'L':
                knots[0].x--;
                break;
            case 'R':
                knots[0].x++;
                break;
            }
            for (int j = 0; j < 9; j++)
            {
                getNewTailPosition(&(knots[j]), &(knots[j + 1]));
            }
            if (!checkPos(&tailPositionList, knots[9].x, knots[9].y))
            {
                addPos(&tailPositionList, knots[9].x, knots[9].y);
            }
        }

    }

    //score
    printf("    Part 2 Score: %d\n", tailPositionList.numElements);

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