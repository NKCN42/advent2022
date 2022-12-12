#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int day6()
{
    int ret = 0;
    FILE* part1 = NULL;
    char buf[4];
    char temp = 0;
    int counter = 0;
    int foundMatch = 0;
    printf("Day 6\n");
    ret = fopen_s(&part1, "day6.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");
    while (temp = fgetc(part1))
    {

        //add to buffer
        buf[counter % 4] = temp;
        counter++;
        if (counter >= 4)
        {
            foundMatch = 0;
            //we need to start checking now
            for (int i = 0; i < 3; i++)
            {
                for (int j = i + 1; j < 4; j++)
                {
                    //if we find a match, set flag
                    if (buf[i] == buf[j])
                    {
                        foundMatch = 1;
                    }
                }
            }
            if (!foundMatch)
            {
                //if we didn't find a match, we have a winner. Cut and print
                printf("     Part 1 End: %d\n", counter);
                break;
            }
        }
    }

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;
}

int day6part2()
{
    int ret = 0;
    FILE* part1 = NULL;
    char buf[14];
    char temp = 0;
    int counter = 0;
    int foundMatch = 0;
    printf("Day 6\n");
    ret = fopen_s(&part1, "day6.txt", "r");
    if (ret != 0)
    {
        printf("     Part 2: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 2: Open successful\n");
    while (temp = fgetc(part1))
    {

        //add to buffer
        buf[counter % 14] = temp;
        counter++;
        if (counter >= 14)
        {
            foundMatch = 0;
            //we need to start checking now
            for (int i = 0; i < 13; i++)
            {
                for (int j = i + 1; j < 14; j++)
                {
                    //if we find a match, set flag
                    if (buf[i] == buf[j])
                    {
                        foundMatch = 1;
                    }
                }
            }
            if (!foundMatch)
            {
                //if we didn't find a match, we have a winner. Cut and print
                printf("     Part 2 End: %d\n", counter);
                break;
            }
        }
    }

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;
}