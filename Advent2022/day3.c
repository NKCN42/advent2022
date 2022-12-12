#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

int day3()
{
	int ret = 0;
	int score = 0;
	FILE* part1 = NULL;
	char buf[128] = { 0 };
	int length = 0; 
	int endfor = 0; 

	printf("Day 3\n");
	ret = fopen_s(&part1, "day3.txt", "r");
	if (ret != 0)
	{
		printf("     Part 1: Failed on fopen: %d\n", ret);
		goto cleanup;
	}
	printf("     Part 1: Open successful\n");

	while (fgets(buf, 128, part1))
	{
		length = strlen(buf); 
		for (int i = 0; i < length / 2; i++)
		{
			for (int j = length / 2; j < length; j++)
			{
				if (buf[i] == buf[j])
				{
					//found the thing, score and break out
					//if buf[i] is an lowercase, 1-26 for a-z
					//else 27-52
					if (buf[i] > 96) //'a' or higher
					{
						//printf("     Part 1: Adding score %c -> %d\n", buf[i], buf[i] - 96); 
						score += buf[i] - 96; 
					}
					else if (buf[i] > 64) //'A' or higher
					{
						//printf("     Part 1: Adding score %c -> %d\n", buf[i], buf[i] - 64);
						score += buf[i] - 64 + 26; 
					}

					endfor = 1; 
					break; 
				}
			}
			if (endfor)
			{
				break; 
			}
		}
		endfor = 0; 
	
	}
	printf("     Part 1 End: Score %d\n", score); 

cleanup: 
	return ret; 
}

int day3part2()
{
	int ret = 0;
	int score = 0;
	FILE* part1 = NULL;
	char buf[128] = { 0 };
	int length = 0;
	int endfor = 0;
	int charcounts[52] = { 0 };
	int tempcounts[52] = { 0 }; 

	printf("Day 3 Part 2\n");
	ret = fopen_s(&part1, "day3.txt", "r");
	if (ret != 0)
	{
		printf("     Part 2: Failed on fopen: %d\n", ret);
		goto cleanup;
	}
	printf("     Part 2: Open successful\n");

	while (fgets(buf, 128, part1))
	{
		//reset charcounts and tempcounts; 
		for (int i = 0; i < 52; i++)
		{
			charcounts[i] = 0;
			tempcounts[i] = 0; 
		}

		//first string is already there. 
		length = strlen(buf); 
		for (int i = 0; i < length; i++)
		{
			if (buf[i] > 96) //'a' or higher
			{
				tempcounts[buf[i] - 97]++;
			}
			else if (buf[i] > 64) //'A' or higher
			{
				
				tempcounts[buf[i] - 65 + 26]++;
			}
		}

		//add to charcounts and reset tempcounts
		for (int i = 0; i < 52; i++)
		{
			if (tempcounts[i] > 0)
			{
				charcounts[i]++; 
			}
			tempcounts[i] = 0; 
		}

		//second string 
		fgets(buf, 128, part1); 
		length = strlen(buf);
		for (int i = 0; i < length; i++)
		{
			if (buf[i] > 96) //'a' or higher
			{
				tempcounts[buf[i] - 97]++;
			}
			else if (buf[i] > 64) //'A' or higher
			{

				tempcounts[buf[i] - 65 + 26]++;
			}
		}

		//add to charcounts and reset tempcounts
		for (int i = 0; i < 52; i++)
		{
			if (tempcounts[i] > 0)
			{
				charcounts[i]++;
			}
			tempcounts[i] = 0;
		}

		//third string 
		fgets(buf, 128, part1);
		length = strlen(buf);
		for (int i = 0; i < length; i++)
		{
			if (buf[i] > 96) //'a' or higher
			{
				tempcounts[buf[i] - 97]++;
			}
			else if (buf[i] > 64) //'A' or higher
			{

				tempcounts[buf[i] - 65 + 26]++;
			}
		}

		//add to charcounts and reset tempcounts
		for (int i = 0; i < 52; i++)
		{
			if (tempcounts[i] > 0)
			{
				charcounts[i]++;
			}
			tempcounts[i] = 0;
		}

		//score
		//printf("     Part 2: Scoring..."); 
		for (int i = 0; i < 52; i++)
		{
			if (charcounts[i] == 3)
			{
				//found it. score and break
				//printf(" Found group common %d\n", i); 
				score += i + 1; 
				break; 
			}
		}

	}
	printf("     Part 2 End: Score %d\n", score);

cleanup:
	return ret;
}