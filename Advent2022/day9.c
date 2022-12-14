#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

typedef struct position
{
    int x;
    int y;
} pos;

int addPos(pos** posList, int x, int y)
{

}

int day9()
{
    int ret = 0;
    FILE* part1 = NULL;
    pos head = { 0 };
    pos tail = { 0 };
    pos* tailPositions = NULL;
    int numTailPos = 0;

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



cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }

    return ret;
}