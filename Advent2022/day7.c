#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "advent.h"

struct day7node
{
    int size;
    char name[16];
    struct day7node* parent;
    struct day7node** children;
    int numChildren;
    int childCapacity;
};

int initializeNode(struct day7node** node, char* name)
{
    int ret = 0;
    struct day7node* temp = *node;
    temp = (struct day7node*)calloc(1, sizeof(struct day7node));
    if (temp == 0)
    {
        ret = 1;
        goto cleanup;
    }
    temp->parent = NULL;
    strncpy_s(temp->name, 16, name, strlen(name));
    temp->size = 0;
    temp->children = (struct day7node**)calloc(2, sizeof(struct day7node*));
    if (temp->children == 0)
    {
        ret = 1;
        goto cleanup;
    }
    temp->childCapacity = 2;
    temp->numChildren = 0;
    *node = temp;
cleanup:
    return ret;
}



int isChildInList(struct day7node** childList, int numChildren, char* name)
{
    int ret = 0;
    for (int i = 0; i < numChildren; i++)
    {
        if (strcmp(childList[i]->name, name) == 0)
        {
            ret = 1;
            break;
        }
    }
    return ret;
}


struct day7node* addChild(struct day7node* parentnode, char* childname)
{
    struct day7node* temp = NULL;
    if (parentnode != NULL && childname != NULL)
    {
        //if child name is not already in there...
        if (!isChildInList(parentnode->children, parentnode->numChildren, childname))
        {
            //check if new number of children would exceed capacity
            if (parentnode->childCapacity < parentnode->numChildren + 1)
            {
                //if it is, realloc the children array
                parentnode->children = realloc(parentnode->children, sizeof(struct day7node*) * (parentnode->childCapacity * 2));
                parentnode->childCapacity *= 2;
            }
            //then add it to the list
            initializeNode(&temp, childname);
            temp->parent = parentnode;
            parentnode->children[parentnode->numChildren] = temp;
            parentnode->numChildren++;
        }
    }
    return temp;
}
int printTree(struct day7node* root, int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
    printf("%s (Size %d)\n", root->name, root->size);
    for (int i = 0; i < root->numChildren; i++)
    {
        printTree(root->children[i], level + 1);
    }
}

int getScore(struct day7node* root)
{
    int ret = 0;
    if (root->size <= 100000)
    {
        ret += root->size;
    }
    for (int i = 0; i < root->numChildren; i++)
    {
        ret += getScore(root->children[i]);
    }
    return ret;
}

struct day7node* getSmallestDeletionCandidate(struct day7node* root, int neededSpace)
{
    struct day7node* ret = NULL;
    struct day7node* temp = NULL;
    if (root->size >= neededSpace)
    {
        //need to assume that root is the smallest candidate to prevent null compares
        ret = root;
        for (int i = 0; i < root->numChildren; i++)
        {
            temp = getSmallestDeletionCandidate(root->children[i], neededSpace);
            //if function returns NULL, move on.
            if (temp != NULL)
            {
                if (temp->size > neededSpace && temp->size < ret->size)
                {
                    ret = temp;
                }
            }

        }
    }
    //do nothing if size of current node is smaller than neededSpace, return NULL because there's no need to traverse further
    return ret;
}

int cleanupChildren(struct day7node* root)
{
    int ret = 0;
    if (root->children != NULL)
    {
        //iterate through children
        for (int i = 0; i < root->numChildren; i++)
        {
            ret += cleanupChildren(root->children[i]);
        }
        free(root->children);
    }
    return ret;
}

int day7()
{
    int ret = 0;
    char* funcret = NULL;
    FILE* part1 = NULL;
    char buf[128] = { 0 };
    struct day7node* cursor = NULL;
    struct day7node* recursor = NULL;
    struct day7node* root = NULL;
    struct day7node* temp = NULL;
    char* context = NULL;
    char* name = NULL;
    char* lsFirstTok = NULL;
    int size = 0;
    int continueLoop = 1;
    printf("Day 7\n");
    ret = fopen_s(&part1, "day7.txt", "r");
    if (ret != 0)
    {
        printf("     Part 1: Failed on fopen: %d\n", ret);
        goto cleanup;
    }
    printf("     Part 1: Open successful\n");

    //intialize root
    if (initializeNode(&root, "/") != 0)
    {
        printf("     Part 1: error on initialize root node");
        ret = 1;
        goto cleanup;
    }
    cursor = root;
    funcret = fgets(buf, 128, part1);
    if (funcret == 0)
    {
        continueLoop = 0;
    }
    while (continueLoop)
    {
        if (strncmp(buf, "$ cd", 4) == 0)
        {
            //tokenize buf by space, ignore first and second
            strtok_s(buf, " ", &context);
            strtok_s(NULL, " ", &context);
            //target directory
            name = strtok_s(NULL, " ", &context);
            if (strncmp(name, "/", 1) == 0)
            {
                //return to root
                cursor = root;
            }
            else if (strncmp(name, "..", 2) == 0)
            {
                //go to parent
                cursor = cursor->parent;
            }
            else
            {
                //go to child of name name
                for (int i = 0; i < cursor->numChildren; i++)
                {
                    if (strcmp((cursor->children[i]->name), name) == 0)
                    {
                        cursor = cursor->children[i];
                        break;
                    }
                }
            }
            funcret = fgets(buf, 128, part1);
            if (funcret == NULL)
            {
                //we're done. break out.
                continueLoop = 0;
            }

        }
        else if (strncmp(buf, "$ ls", 4) == 0)
        {
            //continue to read lines until first character is $
            while (funcret = fgets(buf, 128, part1))
            {
                if (funcret == NULL)
                {
                    continueLoop = 0;
                    break;
                }
                if (buf[0] == '$')
                {
                    break;
                }
                //tokenize buf by space
                lsFirstTok = strtok_s(buf, " ", &context);
                name = strtok_s(NULL, " ", &context);
                if (strncmp(lsFirstTok, "dir", 3) == 0)
                {
                    //name is a directory and we need to add it to the child list at current location
                    addChild(cursor, name);
                }
                else
                {
                    //name is a file and the lsFirstTok is a number that is size in bytes
                    size = strtol(lsFirstTok, NULL, 10);
                    cursor->size += size;
                    recursor = cursor;
                    while (recursor->parent != NULL)
                    {
                        recursor = recursor->parent;
                        recursor->size += size;
                    }
                }
            }
        }
        else
        {
            printf("Unrecognized command! %s\n", buf);
            funcret = fgets(buf, 128, part1);
            if (funcret == NULL)
            {
                continueLoop = 0;
            }
        }
    }

    //printTree(root, 0);
    //Score
    printf("     Part 1 Score: %d\n", getScore(root));

    //Part 2 does not need its own function. It can be easily slotted into this one.
    int totalUsedDisk = root->size;
    int totalFreeSpace = 70000000 - totalUsedDisk;
    int totalNeeded = 30000000 - totalFreeSpace;
    if (totalNeeded > 0)
    {
        //we need more space
        printf("     Part 2: We need %d more space\n", totalNeeded);
        temp = getSmallestDeletionCandidate(root, totalNeeded);
        printf("     Part 2: %s is the smallest directory with size %d\n", temp->name, temp->size);

    }

cleanup:
    if (part1 != NULL)
    {
        fclose(part1);
    }
    if (root != NULL)
    {
        cleanupChildren(root);
        free(root);
    }
    return ret;
}
