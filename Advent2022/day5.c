#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

/*
Yay data structure time
Create node structure
Contents:
    -Value
    -Next
*/
struct node
{
    char value;
    struct node* next;
};


/*
Functions for node structure

enqueue - add to end of node list

push - add to head of list
pop - remove from head of list
pushX - add to head, preserving order
popX - remove from head of list, preserving order

clear - remove all elements

*/

int pushX(struct node** stack, struct node* val)
{
    int ret = 0;
    struct node* temp = NULL;
    //we need to iterate to end of val list, then append stack contents there
    //if stack is empty, just plop it in.
    if (*stack == NULL)
    {
        *stack = val;
    }
    //if stack has something, we have more to do, but not that much.
    else if (val != NULL)
    {
        temp = val;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = *stack;
        (*stack) = val;
    }
    //if val is NULL, there's nothing to do

    return ret;
}

int push(struct node** stack, struct node* val)
{
    int ret = 0;
    //if stack is empty, set and leave (ensure reset of val->next)
    if (*stack == NULL)
    {
        *stack = val;
        (*stack)->next = NULL;
    }
    else
    {
        val->next = *stack;
        *stack = val;
    }
    return ret;
}

struct node* popX(struct node** stack, int numtopop)
{
    struct node* temp = NULL;
    struct node* temp2 = NULL;
    //if stack is empty, do nothing
    if (*stack != NULL)
    {
        temp = *stack;
        temp2 = *stack;
        for (int i = 0; i < numtopop - 1; i++)
        {
            //so long as temp2->next != NULL, count it off
            if (temp2->next == NULL)
            {
                break;
            }
            temp2 = temp2->next;
        }
        //set stack to what's after the scanned area
        *stack = temp2->next;
        temp2->next = NULL;
    }
    return temp;
}

struct node* pop(struct node** stack)
{

    struct node* temp = NULL;
    //if stack is empty, do nothing
    if (*stack != NULL)
    {
        temp = *stack;
        (*stack) = temp->next;
        temp->next = NULL;
    }
    return temp;
}

int enqueue(struct node** node, char val)
{
    int ret = 0;
    struct node* temp = NULL;
    struct node* temp2 = NULL;
    //if node is uninitialized, initialize it and set values
    if (*node == NULL)
    {
        //initialize node
        *node = (struct node*)calloc(1, sizeof(struct node));
        //set values
        (*node)->value = val;
        (*node)->next = NULL;
    }
    //if the next node is NULL, alloc and set
    else if ((*node)->next == NULL)
    {
        temp = (struct node*)calloc(1, sizeof(struct node));
        temp->value = val;
        temp->next = NULL;
        (*node)->next = temp;
    }
    else
    {
        temp = *node;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp2 = (struct node*)calloc(1, sizeof(struct node));
        temp2->value = val;
        temp2->next = NULL;
        temp->next = temp2;
    }

    return ret;

}

int printStacks(struct node* stack, int stackNum)
{
    int ret = 0;
    struct node* temp = stack;

    printf("Stack %d: ", stackNum);
    while (temp != NULL)
    {
        printf("%c ", temp->value);
        temp = temp->next;
    }

    printf("\n");
    return ret;
}

int freeStack(struct node** stack)
{
    int ret = 0;
    struct node* temp = *stack;
    struct node* temp2 = NULL;
    while (temp != NULL)
    {
        temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    return ret;
}

int day5()
{
    int ret = 0;
    int funcret = 0;
    int score = 0;
    FILE* part1 = NULL;
    char buf[128] = { 0 };
    int numStacks = 0;
    int length = 0;
    int counter = 0;
    struct node** stacks = NULL;
    int inputFlag = 1;
    char* context;
    int numToMove = 0;
    int srcStack = 0;
    int dstStack = 0;

    printf("Day 5\n");
    ret = fopen_s(&part1, "day5.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    //The hard part: How to get initial state
    //first line: initialize number of stacks (of type struct node*) based off of strlen
    fgets(buf, 128, part1);
    length = strlen(buf);

    //count number of stacks
    for (int i = 1; i < length; i += 4)
    {
        numStacks++;
    }

    //initialize stacks
    stacks = (struct node**)calloc(numStacks, sizeof(struct node*));

    do
    {
        //process line
        counter = 0;
        for (int i = 1; i < length; i += 4)
        {
            if (buf[i] != ' ')
            {
                enqueue(&stacks[counter], buf[i]);
            }
            counter++;
        }

        //end loop: read next line and determine if we need to loop again
        funcret = fgets(buf, 128, part1);
        if (buf[1] == '1' || funcret == 0)
        {
            inputFlag = 0;
        }
    } while (inputFlag);


    //Phase 2: manipulate stacks
    //skip empty line
    fgets(buf, 128, part1);
    while (fgets(buf, 128, part1))
    {
        //format "move X from Y to Z"
        // X = number of elements to move
        // Y = src
        // Z = dst

        //first token "move", ignore
        strtok_s(buf, " ", &context);

        //second token: X
        numToMove = strtol(strtok_s(NULL, " ", &context), NULL, 10);

        //third token: "from", ignore
        strtok_s(NULL, " ", &context);

        //fourth token: Y
        srcStack = strtol(strtok_s(NULL, " ", &context), NULL, 10) - 1; //sub 1 to align with stacks index

        //fifth token: "to", ignore
        strtok_s(NULL, " ", &context);

        //sixth token: Z
        dstStack = strtol(strtok_s(NULL, " ", &context), NULL, 10) - 1; //sub 1 to align with stacks index

        //do work
        for (int i = 0; i < numToMove; i++)
        {
            push(&stacks[dstStack], pop(&stacks[srcStack]));
        }
    }

    //print stacks
    for (int i = 0; i < numStacks; i++)
    {
        printStacks(stacks[i], i);
    }

    //print answer
    printf("     Part 1 Answer: ");
    for (int i = 0; i < numStacks; i++)
    {
        printf("%c", stacks[i]->value);
    }
    printf("\n");


cleanup:
    if (stacks != NULL)
    {
        //dispose of all stacks
        for (int i = 0; i < numStacks; i++)
        {
            //dispose of single stack
            freeStack(&stacks[i]);
        }
        free(stacks);
    }
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;
}

int day5part2()
{
    int ret = 0;
    int funcret = 0;
    int score = 0;
    FILE* part1 = NULL;
    char buf[128] = { 0 };
    int numStacks = 0;
    int length = 0;
    int counter = 0;
    struct node** stacks = NULL;
    int inputFlag = 1;
    char* context;
    int numToMove = 0;
    int srcStack = 0;
    int dstStack = 0;

    printf("Day 5 Part 2\n");
    ret = fopen_s(&part1, "day5.txt", "r");
    if (ret != 0)
    {
        printf("     Part 2: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 2: Open successful\n");

    //The hard part: How to get initial state
    //first line: initialize number of stacks (of type struct node*) based off of strlen
    fgets(buf, 128, part1);
    length = strlen(buf);

    //count number of stacks
    for (int i = 1; i < length; i += 4)
    {
        numStacks++;
    }

    //initialize stacks
    stacks = (struct node**)calloc(numStacks, sizeof(struct node*));

    do
    {
        //process line
        counter = 0;
        for (int i = 1; i < length; i += 4)
        {
            if (buf[i] != ' ')
            {
                enqueue(&stacks[counter], buf[i]);
            }
            counter++;
        }

        //end loop: read next line and determine if we need to loop again
        funcret = fgets(buf, 128, part1);
        if (buf[1] == '1' || funcret == 0)
        {
            inputFlag = 0;
        }
    } while (inputFlag);
    //print stacks
    printf("Phase 1 End:\n");
    for (int i = 0; i < numStacks; i++)
    {
        printStacks(stacks[i], i);
    }

    //Phase 2: manipulate stacks
    //skip empty line
    fgets(buf, 128, part1);
    counter = 0;
    while (fgets(buf, 128, part1))
    {
        //format "move X from Y to Z"
        // X = number of elements to move
        // Y = src
        // Z = dst
        if (counter == 0)
        {
            printf("%s\n", buf);
        }
        //first token "move", ignore
        strtok_s(buf, " ", &context);

        //second token: X
        numToMove = strtol(strtok_s(NULL, " ", &context), NULL, 10);

        //third token: "from", ignore
        strtok_s(NULL, " ", &context);

        //fourth token: Y
        srcStack = strtol(strtok_s(NULL, " ", &context), NULL, 10) - 1; //sub 1 to align with stacks index

        //fifth token: "to", ignore
        strtok_s(NULL, " ", &context);

        //sixth token: Z
        dstStack = strtol(strtok_s(NULL, " ", &context), NULL, 10) - 1; //sub 1 to align with stacks index

        //do work
        pushX(&stacks[dstStack], popX(&stacks[srcStack], numToMove));
        /*for (int i = 0; i < numToMove; i++)
        {
            push(&stacks[dstStack], pop(&stacks[srcStack]));
        }*/

        //print stacks
        if (counter == 0)
        {
            for (int i = 0; i < numStacks; i++)
            {
                printStacks(stacks[i], i);
            }
            counter++;
        }
    }
    /*
    //print stacks
    for (int i = 0; i < numStacks; i++)
    {
        printStacks(stacks[i], i);
    }*/

    //print answer
    printf("     Part 2 Answer: ");
    for (int i = 0; i < numStacks; i++)
    {
        if (stacks[i] != NULL)
        {
            printf("%c", stacks[i]->value);
        }
        else
        {
            printf(" ");
        }
    }
    printf("\n");


cleanup:
    if (stacks != NULL)
    {
        //dispose of all stacks
        for (int i = 0; i < numStacks; i++)
        {
            //dispose of single stack
            freeStack(&stacks[i]);
        }
        free(stacks);
    }
    if (part1 != NULL)
    {
        fclose(part1);
    }
    return ret;
}