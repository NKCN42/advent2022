#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"
#include "utils.h"


typedef struct day11m {
    char op;
    int left;
    int right;
    int testDivisor;
    int trueTarget;
    int falseTarget;
    vector itemVector; //vector will be storing ints, so we'll have to keep that in mind
} monkey;

void freeMonkey(void* mon)
{
    monkey* monk = (monkey*)mon;
    if (monk->itemVector.items != NULL)
    {
        vectorFree(&(monk->itemVector), NULL);
    }
}

void printMonkey(monkey* mon, int number)
{
    printf("Monkey %d\n", number);
    printf("\tItems: ");
    for (int i = 0; i < mon->itemVector.size; i++)
    {
        printf("%d,", (int)vectorItemAt(&(mon->itemVector), i));
    }
    printf("\n");
    printf("\tOperation: %d %c %d\n", mon->left, mon->op, mon->right);
    printf("\tTest: divisible by %d\n", mon->testDivisor);
    printf("\t\tTrue: goto %d\n", mon->trueTarget);
    printf("\t\tFalse: goto %d\n", mon->falseTarget);
}

int day11()
{
    int ret = 0;
    FILE* part1 = NULL;
    vector monkeyVec = { 0 }; //vector will be storing monkeys
    monkey* tempMonkey = NULL;
    char buf[128] = { 0 };
    char* tempStr = NULL;
    char* context = NULL;
    int itemVal = 0;
    printf("Day 11\n");
    ret = fopen_s(&part1, "day11.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    vectorInit(&monkeyVec);

    //Phase 1: Input Processing
    while (fgets(buf, 128, part1))
    {
        //first line is monkey designation. We just want to print. Also, make a monkey
        //printf("%s\n", buf);
        tempMonkey = (monkey*)calloc(1, sizeof(monkey));
        if (tempMonkey == NULL)
        {
            printf("     Part 1: Error on allocating monkey\n");
            ret = 1;
            goto cleanup;
        }
        vectorInit(&(tempMonkey->itemVector));

        //next line is "Starting Items: num, num, num ..."
        fgets(buf, 128, part1);
        if (buf == NULL)
        {
            //uhhhhhh bad input or I'm bad
            printf("Unexpected EOF (expected Starting Items line)\n");
            ret = 1;
            goto cleanup;
        }
        //First: "Starting", ignore
        tempStr = strtok_s(buf, " ", &context);
        //Second: "Items:", ignore
        tempStr = strtok_s(NULL, " ", &context);
        //Third -> n: "<num>,"
        tempStr = strtok_s(NULL, " ", &context);
        while (tempStr)
        {
            itemVal = strtol(tempStr, NULL, 10);
            vectorAdd(&(tempMonkey->itemVector), (void*)itemVal);
            tempStr = strtok_s(NULL, " ", &context);
        }

        //next line: "Operation: <some mathematical thing that I'll need to parse>"
        //i.e.: new = old * 6, new = old * old, etc.
        fgets(buf, 128, part1);
        if (buf == NULL)
        {
            printf("Unexpected EOF (expected Operation line\n");
            ret = 1;
            goto cleanup;
        }
        //First: "Operation:", ignore
        tempStr = strtok_s(buf, " ", &context);
        //second: "new", ignore
        tempStr = strtok_s(NULL, " ", &context);
        //third: "=", ignore
        tempStr = strtok_s(NULL, " ", &context);
        //fourth: left operand, store
        tempStr = strtok_s(NULL, " ", &context);
        //if "old", the following will still be 0.
        tempMonkey->left = strtol(tempStr, NULL, 10);
        //fifth: operator, store
        tempStr = strtok_s(NULL, " ", &context);
        tempMonkey->op = tempStr[0];
        //sixth: operator
        tempStr = strtok_s(NULL, " ", &context);
        tempMonkey->right = strtol(tempStr, NULL, 10);

        //next line: "Test: divisible by X"
        fgets(buf, 128, part1);
        //First: "Test:", ignore
        tempStr = strtok_s(buf, " ", &context);
        //Second: "divisible", ignore
        tempStr = strtok_s(NULL, " ", &context);
        //Third: "by", ignore
        tempStr = strtok_s(NULL, " ", &context);
        //Fourth: Finally the thing we want
        tempStr = strtok_s(NULL, " ", &context);
        tempMonkey->testDivisor = strtol(tempStr, NULL, 10);

        //next line: "If true: throw to monkey X"
        fgets(buf, 128, part1);
        tempMonkey->trueTarget = buf[25] - '0';

        //next line: "If false: throw to monkey Y"
        fgets(buf, 128, part1);
        tempMonkey->falseTarget = buf[26] - '0';

        //Print Monkey
        printMonkey(tempMonkey, monkeyVec.size);
        vectorAdd(&monkeyVec, (void*)tempMonkey);

        //skip empty line
        fgets(buf, 128, part1);
    }

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    if (monkeyVec.items != NULL)
    {
        vectorFree(&monkeyVec, freeMonkey);
    }
    return ret;
}