//Dylan VanAllen Lab 6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int task2(unsigned char c)
{
	int counter = 0;
	while(c != 0)
	{
		if (c & 1 == 1)
		{
			counter += 1;
		}
		c = c >> 1;
	}
	printf("Number of 1 bits: %d\n", counter);
	return counter;
}

char task3()
{
	unsigned char c = 'O';
	if (task2(c) % 2 == 0)
	{
		//parity even, set parity bit 1
		c = c | 128;
	}
	else
	{
		//parity odd, set parity bit 0
		c = c & 127;
	}
	return c;
}

int task4()//85 is one we want	
{
	int numberset[] = {23, 23, 23, 23, 46, 46, 79, 79, 79, 79, 85, 85, 85};
	int counter=0;
	int oddnum;
	for(int i=0;i<sizeof(numberset);i++)
	{
		counter = 0;
		for(int j=0;j<sizeof(numberset);j++)
		{
			if(numberset[i] ^ numberset[j])
			{
				counter+=1;
			}
		}
		if (counter % 2 > 0)
		{
			oddnum = numberset[i];
			break;
		}
	}
	return oddnum;
}

void task5()
{
	int x = 54;
	unsigned int bitmask = 128;
	printf("%d in binary: ", x);
	while (bitmask > 0)
	{
		if ((x & bitmask) == 0)
		{
			printf("0");
		}
		else
		{
			printf("1");
		}
		bitmask = bitmask >> 1;
	}
	printf("\n");
}


//decryption and encryption exactly the same, invert bits

	
void task6()
{
	FILE *fp1, *fp2;
	char ch;
	
	
	fp1 = fopen("first.txt", "r");
	fp2 = fopen("second.txt", "w");
	while (1) 
	{
		ch = fgetc(fp1);

		if (ch == EOF)
		 break;
		else
		 putc(~ch, fp2);
	}
	 
	printf("File encrypted\n");
	fclose(fp1);
	fclose(fp2);
}

int main()
{
	printf("character with parity bit: %c\n", task3());
	printf("Odd number out: %d\n", task4());
	task5();
	task6();
}
		
	
