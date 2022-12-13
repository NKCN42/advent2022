#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

int day8()
{
    int ret = 0;
    FILE* part1 = NULL;
    int funcret = 0;
    char buf[128];
    char** inputMatrix = NULL;
    char** temp = NULL;
    int numberOfColumns = 0;
    int numberOfRows = 0;
    int inputRowCapacity = 0;
    int exposedTrees = 0;
    char currentTreeHeight = 0;
    char compareHeight = 0;
    int visibility = 0;
    int p2ScoreParts[4] = { 0 };
    int p2HighScore = 0;
    int p2Temp = 0;
    printf("Day 8\n");
    ret = fopen_s(&part1, "day8.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    //Initialize inputMatrix
    inputRowCapacity = 2;
    inputMatrix = (char**)calloc(inputRowCapacity, sizeof(char*));
    printf("     Part 1: Initialized inputMatrix\n");
    //Phase 1: Read input
    //Format lines of numbers (0-9) of arbitrary length (equivalent to number of columns)
    //Arbitrary number of rows of same length as above.

    while (fgets(buf, 128, part1))
    {
        //printf("%s", buf);
        if (numberOfRows == 0)
        {
            //set number of columns
            numberOfColumns = strlen(buf) - 1; //ignore trailing /n
        }
        //allocate new row, resize inputMatrix if necessary
        if (numberOfRows + 1 >= inputRowCapacity)
        {
            inputRowCapacity *= 2;
            temp = realloc(inputMatrix, sizeof(char*) * inputRowCapacity);
            if (temp == NULL)
            {
                //uh bad times...
                ret = 1;
                goto cleanup;
            }
            inputMatrix = temp;
        }
        inputMatrix[numberOfRows] = (char*)calloc(numberOfColumns + 1, sizeof(char));
        if (buf[strlen(buf) - 1] == '\n')
        {
            funcret = strncpy_s(inputMatrix[numberOfRows], numberOfColumns + 1, buf, strlen(buf) - 1);
        }
        else
        {
            funcret = strncpy_s(inputMatrix[numberOfRows], numberOfColumns + 1, buf, strlen(buf));
        }
        //printf("funcret = %d\n", funcret);
        numberOfRows++;

    }

    /*for (int i = 0; i < numberOfRows; i++)
    {
        for (int j = 0; j < numberOfColumns; j++)
        {
            printf("%c ", inputMatrix[i][j]);
        }
        printf("\n");
    }*/

    //Phase 2: Analysis
    //First: all edge trees are already exposed. Add to score. (-1 on columns to prevent double-counting corners
    exposedTrees = (numberOfRows * 2) + ((numberOfColumns - 2) * 2);

    for (int i = 1; i < numberOfRows - 1; i++)
    {
        for (int j = 1; j < numberOfColumns - 1; j++)
        {
            //Part 1 Scoring
            visibility = 4;
            currentTreeHeight = inputMatrix[i][j];
            //check left
            for (int k = 0; k < j; k++)
            {
                compareHeight = inputMatrix[i][k];
                //printf("Left: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    visibility--;
                    break;
                }
                //printf("\n");
            }

            //check right
            for (int k = j + 1; k < numberOfColumns; k++)
            {
                compareHeight = inputMatrix[i][k];
                //printf("Right: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    visibility--;
                    break;
                }
                //printf("\n");
            }
            //check up
            for (int k = 0; k < i; k++)
            {
                compareHeight = inputMatrix[k][j];
                //printf("Up: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    visibility--;
                    break;
                }
                //printf("\n");
            }
            //and down
            for (int k = i + 1; k < numberOfRows; k++)
            {
                compareHeight = inputMatrix[k][j];
                //printf("Down: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    visibility--;
                    break;
                }
                //printf("\n");
            }
            //Part 1 Score
            if (visibility > 0)
            {
                exposedTrees++;
            }

            //Part 2
            //reset score parts
            for (int i = 0; i < 4; i++)
            {
                p2ScoreParts[i] = 0;
            }

            //check left
            for (int k = j - 1; k >= 0; k--)
            {
                compareHeight = inputMatrix[i][k];
                //printf("Left: Comparing %c to %c...", compareHeight, currentTreeHeight);
                p2ScoreParts[0]++;
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    break;
                }
                //printf("\n");
            }

            //check right
            for (int k = j + 1; k < numberOfColumns; k++)
            {
                compareHeight = inputMatrix[i][k];
                p2ScoreParts[1]++;
                //printf("Right: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    break;
                }
                //printf("\n");
            }
            //check up
            for (int k = i - 1; k >= 0; k--)
            {
                compareHeight = inputMatrix[k][j];
                p2ScoreParts[2]++;
                //printf("Up: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    break;
                }
                //printf("\n");
            }
            //and down
            for (int k = i + 1; k < numberOfRows; k++)
            {
                compareHeight = inputMatrix[k][j];
                p2ScoreParts[3]++;
                //printf("Down: Comparing %c to %c...", compareHeight, currentTreeHeight);
                if (compareHeight >= currentTreeHeight)
                {
                    //printf("Tree should shroud\n");
                    break;
                }
                //printf("\n");
            }
            //Part 2 Scoring
            p2Temp = p2ScoreParts[0] * p2ScoreParts[1] * p2ScoreParts[2] * p2ScoreParts[3];
            if (p2Temp > p2HighScore)
            {
                p2HighScore = p2Temp;
            }
        }
    }

    printf("     Part 1: Exposed trees = %d\n", exposedTrees);
    printf("     Part 2: High Score = %d\n", p2HighScore);

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    if (inputMatrix != NULL)
    {
        for (int i = 0; i < numberOfRows; i++)
        {
            //free individual rows
            free(inputMatrix[i]);
        }
        free(inputMatrix);
    }
}