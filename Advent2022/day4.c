#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

int day4()
{
    int ret = 0;
    int score = 0;
    FILE* part1 = NULL;
    char buf[128] = { 0 };
    char* elf1Range = NULL;
    char* elf2Range = NULL;
    int length = 0;
    int commaIndex;
    int elf1Start = 0;
    int elf2Start = 0;
    int elf1End = 0;
    int elf2End = 0;
    char* context = NULL;

    printf("Day 4\n");
    ret = fopen_s(&part1, "day4.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    while (fgets(buf, 128, part1))
    {
        /*we need to do the following: */
        //splice by ,
        length = strlen(buf);
        elf1Range = buf;
        for (commaIndex = 0; commaIndex < length; commaIndex++)
        {
            if (buf[commaIndex] == ',')
                break;
        }
        buf[commaIndex] = 0;
        elf2Range = buf + commaIndex + 1;
        //splice parts by -
        //strtol the 4 parts, to create ranges
        elf1Start = strtol(strtok_s(elf1Range, "-", &context), NULL, 10);
        elf1End = strtol(strtok_s(NULL, "-", &context), NULL, 10);
        elf2Start = strtol(strtok_s(elf2Range, "-", &context), NULL, 10);
        elf2End = strtol(strtok_s(NULL, "-", &context), NULL, 10);


        /*perform math(the easy part)
            basically, check if elf1.start <=elf2.start && elf1.end >= elf2.end and vice versa
            if so, add 1 to score
        */
        if (elf1Start <= elf2Start && elf1End >= elf2End)
        {
            //printf("     Part 1: Hit! Elf1 (%d-%d), Elf2 (%d-%d)\n", elf1Start, elf1End, elf2Start, elf2End);
            score++;
        }
        else if (elf2Start <= elf1Start && elf2End >= elf1End)
        {
            //printf("     Part 1: Hit! Elf1 (%d-%d), Elf2 (%d-%d)\n", elf1Start, elf1End, elf2Start, elf2End);
            score++;
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


int day4part2()
{
    int ret = 0;
    int score = 0;
    FILE* part1 = NULL;
    char buf[128] = { 0 };
    char* elf1Range = NULL;
    char* elf2Range = NULL;
    int length = 0;
    int commaIndex;
    int elf1Start = 0;
    int elf2Start = 0;
    int elf1End = 0;
    int elf2End = 0;
    char* context = NULL;

    printf("Day 4\n");
    ret = fopen_s(&part1, "day4.txt", "r");
    if (ret != 0)
    {
        printf("     Part 2: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 2: Open successful\n");

    while (fgets(buf, 128, part1))
    {
        /*we need to do the following: */
        //splice by ,
        length = strlen(buf);
        elf1Range = buf;
        for (commaIndex = 0; commaIndex < length; commaIndex++)
        {
            if (buf[commaIndex] == ',')
                break;
        }
        buf[commaIndex] = 0;
        elf2Range = buf + commaIndex + 1;
        //splice parts by -
        //strtol the 4 parts, to create ranges
        elf1Start = strtol(strtok_s(elf1Range, "-", &context), NULL, 10);
        elf1End = strtol(strtok_s(NULL, "-", &context), NULL, 10);
        elf2Start = strtol(strtok_s(elf2Range, "-", &context), NULL, 10);
        elf2End = strtol(strtok_s(NULL, "-", &context), NULL, 10);


        /*perform math(the easy part)
            if any overlap, add to score
            if so, add 1 to score
        */
        if (elf1Start >= elf2Start && elf1Start <= elf2End)
        {
            //printf("     Part 2: Hit! Elf1 (%d-%d), Elf2 (%d-%d)\n", elf1Start, elf1End, elf2Start, elf2End);
            score++;
        }
        else if (elf2Start >= elf1Start && elf2Start <= elf1End)
        {
            //printf("     Part 2: Hit! Elf1 (%d-%d), Elf2 (%d-%d)\n", elf1Start, elf1End, elf2Start, elf2End);
            score++;
        }
    }

    printf("     Part 2 Score: %d\n", score);

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;
}