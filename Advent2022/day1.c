#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "advent.h"

int day1()
{
	int ret = 0; 
	FILE* part1 = NULL; 
	char buf[1024] = { 0 };
	int mostCalories = 0; 
	int runningTotal = 0; 
	int lineInt = 0; 

	printf("Day 1\n"); 
	ret = fopen_s(&part1, "day1-1.txt", "r"); 
	if (ret != 0)
	{
		printf("     Part 1: Failed on fopen: %d\n", ret); 
		goto cleanup; 
	}
	printf("     Part 1: Open successful\n");
	
	while (fgets(buf, 1024, part1))
	{
		//if buf = "\n", set variables and continue
		if (strcmp(buf, "\n") == 0)
		{
			if (runningTotal > mostCalories)
			{
				mostCalories = runningTotal; 
				//printf("     Part 1: New mostCalories = %d\n", mostCalories);
			}
			runningTotal = 0; 
		}
		else
		{
			//strtol
			lineInt = strtol(buf, NULL, 10); 
			runningTotal += lineInt; 
		}
	}

	printf("     Part 1 End: Most Calories = %d\n", mostCalories); 



cleanup: 
	if (part1 != NULL)
	{
		fclose(part1); 
	}
	return ret; 
}

int day1part2()
{
	int ret = 0;
	FILE* part1 = NULL;
	char buf[1024] = { 0 };
	//mostCalories now has to be a sorted list, where the least of the greatest is in index 0, and so on until index 2 being the most
	int mostCalories[3] = { 0 };
	int runningTotal = 0;
	int lineInt = 0;

	printf("Day 1 Part 2\n");
	ret = fopen_s(&part1, "day1-1.txt", "r");
	if (ret != 0)
	{
		printf("     Part 2: Failed on fopen: %d\n", ret);
		goto cleanup;
	}
	printf("     Part 2: Open successful\n");

	while (fgets(buf, 1024, part1))
	{
		//if buf = "\n", set variables and continue
		if (strcmp(buf, "\n") == 0)
		{
			//if this check is true, we actually need to check due to the sorted nature of the list
			if (runningTotal > mostCalories[0])
			{
				if (runningTotal > mostCalories[1])
				{
					if (runningTotal > mostCalories[2])
					{
						//new big mamajama, shove to the left all the way
						//printf("     Part 2: mostCalories[2] updated with value %d\n", runningTotal); 
						mostCalories[0] = mostCalories[1]; 
						mostCalories[1] = mostCalories[2]; 
						mostCalories[2] = runningTotal; 
					}
					else
					{
						//replace index 1 with runningTotal
						//printf("     Part 2: mostCalories[1] updated with value %d\n", runningTotal);
						mostCalories[0] = mostCalories[1]; 
						mostCalories[1] = runningTotal; 
					}
				}
				else
				{
					//replace index 0
					//printf("     Part 2: mostCalories[0] updated with value %d\n", runningTotal);
					mostCalories[0] = runningTotal; 
				}
			}
			runningTotal = 0;
		}
		else
		{
			//strtol
			lineInt = strtol(buf, NULL, 10);
			runningTotal += lineInt;
		}
	}

	printf("     Part 2 End: Most Calories:\n");
	for (int i = 0; i < 3; i++)
	{
		printf("        [%d] = %d\n", i, mostCalories[i]); 
	}
	printf("        Total = %d\n", mostCalories[0] + mostCalories[1] + mostCalories[2]);



cleanup:
	if (part1 != NULL)
	{
		fclose(part1);
	}
	return ret;
}