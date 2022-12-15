#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

enum commandType {
    NOOP,
    ADDX
};

int day10()
{
    int ret = 0;
    FILE* part1 = NULL;
    int cycleCount = 0;
    int commandCycles = 0;
    int value = 1;
    int score = 0;
    char buf[32];
    char* command = NULL;
    char* temp = NULL;
    char* context;
    int commandVal = 0;
    enum commandType cmdType = NOOP;

    printf("Day 10\n");
    ret = fopen_s(&part1, "day10.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    while (fgets(buf, 32, part1))
    {
        //token out the thing
        //printf("%s\n", buf);
        command = strtok_s(buf, " ", &context);
        temp = strtok_s(NULL, " ", &context);
        if (temp != NULL)
        {
            commandVal = strtol(temp, NULL, 10);
        }
        else
        {
            commandVal = 0;
        }
        commandCycles = 0;
        if (strncmp(command, "noop", 4) == 0)
        {
            commandCycles++;
            cmdType = NOOP;
        }
        else if (strncmp(command, "addx", 4) == 0)
        {
            commandCycles = 2;
            cmdType = ADDX;
        }
        for (int i = 0; i < commandCycles; i++)
        {
            if (cycleCount % 40 >= value - 1 && cycleCount % 40 <= value + 1)
            {
                printf("#");
            }
            else
            {
                printf(".");
            }
            cycleCount++;
            if (cycleCount % 40 == 0)
            {
                printf("\n");
            }
            switch (cycleCount)
            {
            default:
                break;
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                score += cycleCount * value;
                break;
            }
        }
        if (cmdType == ADDX)
        {
            value += commandVal;
        }

    }

    printf("     Part 1 Score: %d\n", score);
cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;

}