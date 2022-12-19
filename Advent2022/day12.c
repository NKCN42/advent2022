#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

int minStepsToEndFromPoint(int startingX, int startingY, int endingX, int endingY, char** map, int rowCount, int columnCount)
{
    char** visited = NULL;
    int** distVal = NULL;
    int ret = 0;
    int i = 0;
    int j = 0;
    int chosenX = 0;
    int chosenY = 0;
    int count = 0;
    int minDistance = 0;
    //Phase 2: Dijkstra's algo for shortest path (man it's been a minute)
    //1) Create a shortest path tree set that keeps track of vertices included
    //   in the shortest path tree.
    visited = (char**)calloc(rowCount, sizeof(char*));
    if (visited == NULL)
    {
        printf("     Part 1: Error allocating visited array\n");
        ret = -1;
        goto cleanup;
    }
    for (i = 0; i < rowCount; i++)
    {
        visited[i] = (char*)calloc(columnCount, sizeof(char));
        if (visited[i] == NULL)
        {
            printf("    Part 1: Error allocating visited array member %d\n", i);
            ret = -1;
            goto cleanup;
        }
    }
    /*2) Assign distance value to all verticies in the input graph. Initialize all
         distance values as infinite. Source node is assigned 0 so it's picked first*/
    distVal = (int**)calloc(rowCount, sizeof(int*));
    if (distVal == NULL)
    {
        printf("     Part 1: Error allocating distVal array\n");
        ret = 1;
        goto cleanup;
    }
    for (i = 0; i < rowCount; i++)
    {
        distVal[i] = (int*)calloc(columnCount, sizeof(int));
        if (distVal[i] == NULL)
        {
            printf("     Part 1: Error allocating distVal array member %d\n", i);
            ret = -1;
            goto cleanup;
        }
        for (j = 0; j < columnCount; j++)
        {
            distVal[i][j] = INT_MAX;
        }
        //if starting point is in this row, assign that to 0 so it will be chosen first.
        if (i == startingX)
        {
            distVal[i][startingY] = 0;
        }
    }
    /*while visited map isn't full (or we haven't found E yet)
      Pick a point which hasn't incremented visited[i][j], which is also the
      minimum distance value in the map (but at least equal to the last thing we checked
    */

    //printf("Starting node (%d, %d)\n", startingX, startingY);
    while (count < (rowCount * columnCount) - 1)
    {
        //select next node
        minDistance = INT_MAX;
        for (i = 0; i < rowCount; i++)
        {
            for (j = 0; j < columnCount; j++)
            {
                if (distVal[i][j] < minDistance && !visited[i][j])
                {
                    minDistance = distVal[i][j];
                    chosenX = i;
                    chosenY = j;
                    //printf("(%d, %d) Chosen!\n", chosenX, chosenY);
                }
            }
        }
        visited[chosenX][chosenY] = 1;
        //check adjacent nodes (as those are the only ones that will be available.
        //up
        if (chosenX + 1 < rowCount && !visited[chosenX + 1][chosenY] && map[chosenX + 1][chosenY] <= map[chosenX][chosenY] + 1)
        {
            if (minDistance + 1 < distVal[chosenX + 1][chosenY])
            {
                //printf("    Setting (%d, %d) distance to %d\n", chosenX + 1, chosenY, minDistance + 1);
                distVal[chosenX + 1][chosenY] = minDistance + 1;
            }
        }
        //down
        if (chosenX - 1 >= 0 && !visited[chosenX - 1][chosenY] && map[chosenX - 1][chosenY] <= map[chosenX][chosenY] + 1)
        {
            if (minDistance + 1 < distVal[chosenX - 1][chosenY])
            {
                //printf("    Setting (%d, %d) distance to %d\n", chosenX - 1, chosenY, minDistance + 1);
                distVal[chosenX - 1][chosenY] = minDistance + 1;
            }
        }
        //left
        if (chosenY - 1 >= 0 && !visited[chosenX][chosenY - 1] && map[chosenX][chosenY - 1] <= map[chosenX][chosenY] + 1)
        {
            if (minDistance + 1 < distVal[chosenX][chosenY - 1])
            {
                //printf("    Setting (%d, %d) distance to %d\n", chosenX, chosenY - 1, minDistance + 1);
                distVal[chosenX][chosenY - 1] = minDistance + 1;
            }
        }
        //right
        if (chosenY + 1 < columnCount && !visited[chosenX][chosenY + 1] && map[chosenX][chosenY + 1] <= map[chosenX][chosenY] + 1)
        {
            if (minDistance + 1 < distVal[chosenX][chosenY + 1])
            {
                //printf("    Setting (%d, %d) distance to %d\n", chosenX, chosenY + 1, minDistance + 1);
                distVal[chosenX][chosenY + 1] = minDistance + 1;
            }
        }
        count++;
    }
    ret = distVal[endingX][endingY];
cleanup:
    //distVal, visited
    if (visited != NULL)
    {
        for (int i = 0; i < rowCount; i++)
        {
            free(visited[i]);
        }
        free(visited);
    }
    if (distVal != NULL)
    {
        for (int i = 0; i < rowCount; i++)
        {
            free(distVal[i]);
        }
        free(distVal);
    }
    return ret;

}

int day12()
{
    int ret = 0;
    FILE* part1 = NULL;
    char buf[256] = { 0 };
    char loopFlag = 1;
    char** map = NULL;
    char** tempMap = NULL;
    char* tempStr = NULL;
    int mapCapacity = 0;
    int columnCount = 0;
    int rowCount = 0;
    //these will need to be preserved after initial setting during input processing.
    int startingX = 0;
    int startingY = 0;
    int endingX = 0;
    int endingY = 0;
    int i = 0;
    int j = 0;
    int minFromAnyA = INT_MAX;
    int tempMin = 0;
    printf("Day 12\n");
    ret = fopen_s(&part1, "day12.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");
    map = (char**)calloc(4, sizeof(char*));
    mapCapacity = 4;
    //Phase 1: Input processing
    while (fgets(buf, 256, part1))
    {
        if (rowCount == 0)
        {
            columnCount = strlen(buf) - 1; //strip /n

        }
        if (rowCount == mapCapacity)
        {
            //realloc map
            tempMap = realloc(map, mapCapacity * 2 * sizeof(char*));
            if (tempMap == NULL)
            {
                printf("     Part 1: Failed realloc!\n");
                ret = 1;
                goto cleanup;
            }
            map = tempMap;
            mapCapacity *= 2;
        }
        tempStr = (char*)calloc(columnCount + 1, sizeof(char));
        strncpy_s(tempStr, columnCount + 1, buf, columnCount);
        map[rowCount] = tempStr;
        for (j = 0; j < columnCount; j++)
        {
            if (tempStr[j] == 'S')
            {
                startingX = rowCount;
                startingY = j;
                tempStr[j] = 'a';
            }
            if (tempStr[j] == 'E')
            {
                endingX = rowCount;
                endingY = j;
                tempStr[j] = 'z';
            }
        }
        rowCount++;

    }

    minFromAnyA = minStepsToEndFromPoint(startingX, startingY, endingX, endingY, map, rowCount, columnCount);
    printf("     Part 1 Score: %d\n", minFromAnyA);

    //Part 2:
    //go through the map. If 'a', send the point through the thing.
    for (i = 0; i < rowCount; i++)
    {
        for (j = 0; j < columnCount; j++)
        {
            if (map[i][j] == 'a')
            {
                tempMin = minStepsToEndFromPoint(i, j, endingX, endingY, map, rowCount, columnCount);
                if (tempMin < minFromAnyA)
                {
                    minFromAnyA = tempMin;
                }
            }
        }
    }
    printf("     Part 2 Score: %d\n", minFromAnyA);

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    if (map != NULL)
    {
        //loop through size and free things
        for (int i = 0; i < rowCount; i++)
        {
            free(map[i]);
        }
        free(map);
    }
    return ret;

}