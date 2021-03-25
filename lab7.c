//Dylan VanAllen Lab 7 - Kernel Files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h> 

void task1()
{
	srand(time(NULL));
	int array[100];
	int randnum = 0, numsum = 0;
	char newchar[2];
	for (int i=0; i<100; i++)
	{
		randnum = rand() % 100;
		array[i] = randnum;
	}
	for (int i=0; i<100; i++)
	{
		numsum += array[i];
	}
	char str[100];
	sprintf(str, "%d", numsum);
	char filename[] = "numbers.";
	strcat(filename, str);
	FILE *fp = fopen(filename, "w");
	for (int i=0; i<100; i++)
	{
		sprintf(newchar, "%d", array[i]);
		if (array[i] > 9)
		{
			putc(newchar[0], fp);
			putc(newchar[1], fp);
			putc('\n', fp);
		}
		else
		{
			putc(newchar[0], fp);
			putc('\n', fp);
		}
	}
	fclose(fp);
}

//task 2 - to sum
int main (void)
{ 
    
	task1();
	struct dirent *de;  
	
    DIR *dr = opendir("."); 
  
    if (dr == NULL) 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
   
    while ((de = readdir(dr)) != NULL) 
    {
		int isequal = strncmp(de->d_name, "numbers.", 8);
		int sum = 0;
		if (isequal == 0)
		{
			FILE *in_file;
			int number;

			in_file = fopen(de->d_name, "r");

			if (in_file == NULL)
			{
				printf("Can't open file for reading.\n");
			}
			else
			{
				for (int i=0;i<100;i++) 	
				{
					fscanf(in_file, "%d", &number);
					sum += number;
				}
				printf("File: %s, Sum: %d\n", de->d_name, sum);
				fclose(in_file);
			}
		}
	}	
    closedir(dr);     
    return 0; 
} 
