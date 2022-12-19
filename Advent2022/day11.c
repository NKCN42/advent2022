#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "advent.h"
#include "utils.h"


typedef struct day11m {
    char op;
    uint64_t left;
    uint64_t right;
    uint64_t testDivisor;
    int trueTarget;
    int falseTarget;
    int numItemsProcessed;
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
    printf("\tOperation: %" PRIu64 " %c %" PRIu64 "\n", mon->left, mon->op, mon->right);
    printf("\tTest: divisible by %" PRIu64"\n", mon->testDivisor);
    printf("\t\tTrue: goto %d\n", mon->trueTarget);
    printf("\t\tFalse: goto %d\n", mon->falseTarget);
}

int day11()
{
    int ret = 0;
    FILE* part1 = NULL;
    vector monkeyVec = { 0 }; //vector will be storing monkeys
    monkey* tempMonkey = NULL;
    monkey* targetMonkey = NULL;
    char buf[128] = { 0 };
    char* tempStr = NULL;
    char* context = NULL;
    int itemVal = 0;
    int highest[2] = { 0 };
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
        tempMonkey->numItemsProcessed = 0;

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
        tempMonkey->trueTarget = buf[29] - '0';

        //next line: "If false: throw to monkey Y"
        fgets(buf, 128, part1);
        tempMonkey->falseTarget = buf[30] - '0';

        //Print Monkey
        //printMonkey(tempMonkey, monkeyVec.size);
        vectorAdd(&monkeyVec, (void*)tempMonkey);

        //skip empty line
        fgets(buf, 128, part1);
    }

    //Phase 2: Rounds of crap slinging
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < monkeyVec.size; j++)
        {
            tempMonkey = (monkey*)vectorItemAt(&monkeyVec, j);
            //throw all the items
            int k = tempMonkey->itemVector.size;
            for (; k > 0; k--)
            {
                tempMonkey->numItemsProcessed++;
                //pop from the head of the vector
                int tempVal = vectorRemove(&tempMonkey->itemVector, 0);
                int leftOperand = tempMonkey->left;
                int rightOperand = tempMonkey->right;
                if (leftOperand == 0)
                {
                    leftOperand = tempVal;
                }
                if (rightOperand == 0)
                {
                    rightOperand = tempVal;
                }
                switch (tempMonkey->op)
                {
                case '+':
                    tempVal = leftOperand + rightOperand;
                    break;
                case '-':
                    tempVal = leftOperand - rightOperand;
                    break;
                case '*':
                    tempVal = leftOperand * rightOperand;
                    break;
                }
                tempVal /= 3;
                if (tempVal % tempMonkey->testDivisor == 0)
                {
                    //true target
                    targetMonkey = (monkey*)vectorItemAt(&monkeyVec, tempMonkey->trueTarget);
                    vectorAdd(&(targetMonkey->itemVector), tempVal);
                }
                else
                {
                    //false target
                    targetMonkey = (monkey*)vectorItemAt(&monkeyVec, tempMonkey->falseTarget);
                    vectorAdd(&(targetMonkey->itemVector), tempVal);
                }
            }
        }
    }

    //Score Part 1
    for (int i = 0; i < monkeyVec.size; i++)
    {
        tempMonkey = vectorItemAt(&monkeyVec, i);
        //highest is sorted from least to greatest because I say so
        if (tempMonkey->numItemsProcessed > highest[0])
        {
            if (tempMonkey->numItemsProcessed > highest[1])
            {
                highest[0] = highest[1];
                highest[1] = tempMonkey->numItemsProcessed;
            }
            else
            {
                highest[0] = tempMonkey->numItemsProcessed;
            }
        }
    }


    printf("     Part 1 Score: %d\n", highest[0] * highest[1]);

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

int day11part2()
{
    int ret = 0;
    FILE* part1 = NULL;
    vector monkeyVec = { 0 }; //vector will be storing monkeys
    monkey* tempMonkey = NULL;
    monkey* targetMonkey = NULL;
    int modulo = 1;
    char buf[128] = { 0 };
    char* tempStr = NULL;
    char* context = NULL;
    uint64_t itemVal = 0;
    uint64_t highest[2] = { 0 };
    printf("Day 11 Part 2\n");
    ret = fopen_s(&part1, "day11.txt", "r");
    if (ret != 0)
    {
        printf("     Part 2: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 2: Open successful\n");

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
        tempMonkey->numItemsProcessed = 0;

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
        modulo *= tempMonkey->testDivisor;
        //next line: "If true: throw to monkey X"
        fgets(buf, 128, part1);
        tempMonkey->trueTarget = buf[29] - '0';

        //next line: "If false: throw to monkey Y"
        fgets(buf, 128, part1);
        tempMonkey->falseTarget = buf[30] - '0';

        //Print Monkey
        //printMonkey(tempMonkey, monkeyVec.size);
        vectorAdd(&monkeyVec, (void*)tempMonkey);

        //skip empty line
        fgets(buf, 128, part1);
    }

    for (int i = 0; i < monkeyVec.size; i++)
    {
        tempMonkey = vectorItemAt(&monkeyVec, i);
        printMonkey(tempMonkey, i);
    }

    //Phase 2: Rounds of crap slinging
    for (int i = 0; i < 10000; i++)
    {
        if (i % 1000 == 0)
        {
            printf("Run %d\n", i);
            for (int l = 0; l < monkeyVec.size; l++)
            {
                tempMonkey = vectorItemAt(&monkeyVec, l);
                printMonkey(tempMonkey, l);
            }
        }
        for (int j = 0; j < monkeyVec.size; j++)
        {
            tempMonkey = (monkey*)vectorItemAt(&monkeyVec, j);
            //throw all the items
            int k = tempMonkey->itemVector.size;
            for (; k > 0; k--)
            {
                tempMonkey->numItemsProcessed++;
                //pop from the head of the vector
                uint64_t tempVal = vectorRemove(&tempMonkey->itemVector, 0);
                uint64_t leftOperand = tempMonkey->left;
                uint64_t rightOperand = tempMonkey->right;
                if (leftOperand == 0)
                {
                    leftOperand = tempVal;
                }
                if (rightOperand == 0)
                {
                    rightOperand = tempVal;
                }
                switch (tempMonkey->op)
                {
                case '+':
                    tempVal = leftOperand + rightOperand;
                    break;
                case '-':
                    tempVal = leftOperand - rightOperand;
                    break;
                case '*':
                    tempVal = leftOperand * rightOperand;
                    break;
                }
                tempVal = tempVal % modulo;
                //tempVal /= 3; oof this is gonna suck
                if (i % 1000 == 0)
                {
                    printf("%" PRIu64 " %c %" PRIu64 " = %" PRIu64 "\n", leftOperand, tempMonkey->op, rightOperand, tempVal);
                }
                if (tempVal % tempMonkey->testDivisor == 0)
                {
                    //true target
                    targetMonkey = (monkey*)vectorItemAt(&monkeyVec, tempMonkey->trueTarget);
                    vectorAdd(&(targetMonkey->itemVector), tempVal);
                }
                else
                {
                    //false target
                    targetMonkey = (monkey*)vectorItemAt(&monkeyVec, tempMonkey->falseTarget);
                    vectorAdd(&(targetMonkey->itemVector), tempVal);
                }
            }
        }
    }

    //Score Part 1
    for (int i = 0; i < monkeyVec.size; i++)
    {
        tempMonkey = vectorItemAt(&monkeyVec, i);
        //highest is sorted from least to greatest because I say so
        if (tempMonkey->numItemsProcessed > highest[0])
        {
            if (tempMonkey->numItemsProcessed > highest[1])
            {
                highest[0] = highest[1];
                highest[1] = tempMonkey->numItemsProcessed;
            }
            else
            {
                highest[0] = tempMonkey->numItemsProcessed;
            }
        }
    }

    printf("     Part 2 Score: %" PRIu64 " * %" PRIu64 " = %" PRIu64 "\n", highest[0], highest[1], highest[0] * highest[1]);

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