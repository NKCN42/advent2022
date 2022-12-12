#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "advent.h"


int day2()
{
	int ret = 0; 
	int score = 0; 
	FILE* part1 = NULL; 
	char buf[32] = { 0 }; 
	char* me = NULL; 
	char* them = NULL; 
	char* next_token = NULL; 

	printf("Day 2\n");
	ret = fopen_s(&part1, "day2.txt", "r");
	if (ret != 0)
	{
		printf("     Part 1: Failed on fopen: %d\n", ret);
		goto cleanup;
	}
	printf("     Part 1: Open successful\n");

	while (fgets(buf, 32, part1))
	{
		//token out the two sides
		them = strtok_s(buf, " ", &next_token); 
		me = strtok_s(NULL, " ", &next_token);

		switch (me[0])
		{
		case 'X': //ROCK 
			switch (them[0])
			{
			case 'A': //ROCK
				//Tie
				score += 3 + 1; 
				break; 
			case 'B': //PAPER
				//Loss
				score += 0 + 1; 
				break; 
			case 'C': //SCISSORS
				//Win 
				score += 6 + 1; 
				break; 
			}
			break; 
		case 'Y': //PAPER
			switch (them[0])
			{
			case 'A': //ROCK
				//Win
				score += 6 + 2;
				break;
			case 'B': //PAPER
				//Tie
				score += 3 + 2;
				break;
			case 'C': //SCISSORS
				//Loss 
				score += 0 + 2;
				break;
			}
			break; 
		case 'Z': //SCISSORS
			switch (them[0])
			{
			case 'A': //ROCK
				//Loss
				score += 0 + 3;
				break;
			case 'B': //PAPER
				//Win
				score += 6 + 3;
				break;
			case 'C': //SCISSORS
				//Tie 
				score += 3 + 3;
				break;
			}
			break; 
		}
	}

	printf("      Part 1 End: Score: %d\n", score); 

cleanup: 
	if (part1 != NULL)
	{
		fclose(part1); 
	}
	return ret; 
}

int day2part2()
{
	int ret = 0;
	int score = 0;
	FILE* part1 = NULL;
	char buf[32] = { 0 };
	char* me = NULL;
	char* them = NULL;
	char* next_token = NULL;

	printf("Day 2 Part 2\n");
	ret = fopen_s(&part1, "day2.txt", "r");
	if (ret != 0)
	{
		printf("     Part 2: Failed on fopen: %d\n", ret);
		goto cleanup;
	}
	printf("     Part 2: Open successful\n");

	while (fgets(buf, 32, part1))
	{
		//token out the two sides
		them = strtok_s(buf, " ", &next_token);
		me = strtok_s(NULL, " ", &next_token);

		switch (me[0])
		{
		case 'X': //Loss 
			switch (them[0])
			{
			case 'A': //ROCK
				//Loss means Scissors
				score += 0 + 3;
				break;
			case 'B': //PAPER
				//Loss means Rock
				score += 0 + 1;
				break;
			case 'C': //SCISSORS
				//Loss means paper
				score += 0 + 2;
				break;
			}
			break;
		case 'Y': //Draw
			switch (them[0])
			{
			case 'A': //ROCK
				score += 3 + 1;
				break;
			case 'B': //PAPER
				score += 3 + 2;
				break;
			case 'C': //SCISSORS
				score += 3 + 3;
				break;
			}
			break;
		case 'Z': //Win
			switch (them[0])
			{
			case 'A': //ROCK
				//Win means paper
				score += 6 + 2;
				break;
			case 'B': //PAPER
				//Win means scissors
				score += 6 + 3;
				break;
			case 'C': //SCISSORS
				//Win means Rock 
				score += 6 + 1;
				break;
			}
			break;
		}
	}

	printf("      Part 2 End: Score: %d\n", score);

cleanup:
	if (part1 != NULL)
	{
		fclose(part1);
	}
	return ret;
}