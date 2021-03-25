//Dylan VanAllen Battleship
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define numships 5
int board[10][10];
int coordinate[2];

int teardown()
{
	for (int i=0;i<10;i++)
	{
		for (int j=0;j<10;j++)
		{
			board[i][j] = 0;
		}
	}
}

void initialize()
{
	int sizeofship;
	int is_vertical = 0;
	int randx = 0;
	int randy = 0;
	int intersection = 0;
	do
	{
		intersection = 0;
		teardown();
		for (int n = 1; n < numships+1; n++)
		{
			switch (n)
			{
				case 1:
					sizeofship = 5;
					break;
				case 2:
					sizeofship = 4;
					break;
				case 3:
					sizeofship = 3;
					break;
				case 4:
					sizeofship = 3;
					break;
				case 5:
					sizeofship = 2;
					break;
				default:
					break;
			}
			if (rand() % 2 == 1)
			{
				is_vertical = 1;
			}
			else
			{
				is_vertical = 0;
			}
			if (is_vertical)
			{
				randx = rand() % 10;
				randy = rand() % (10-sizeofship)+1;
				for (int m = 0; m < sizeofship; m++)
				{
					if (board[randx][randy+m] == 0)
					{
						board[randx][randy+m] = n;
					}
					else
					{
						intersection = 1;
					}
				}
			}
			else
			{
				randx = rand() % (10-sizeofship)+1;
				randy = rand() % 10;
				for (int m = 0; m < sizeofship; m++)
				{
					if (board[randx+m][randy] == 0)
					{
						board[randx+m][randy] = n;
					}
					else
					{
						intersection = 1;
					}
				}
			}
		}
		
	} while(intersection == 1);
}
					
void inp(char input[3])
{
	switch (toupper(input[0]))
	{
		case 'A':
			coordinate[0] = 0;
			break;
		case 'B':
			coordinate[0] = 1;
			break;
		case 'C':
			coordinate[0] = 2;
			break;
		case 'D':
			coordinate[0] = 3;
			break;
		case 'E':
			coordinate[0] = 4;
			break;
		case 'F':
			coordinate[0] = 5;
			break;
		case 'G':
			coordinate[0] = 6;
			break;
		case 'H':
			coordinate[0] = 7;
			break;
		case 'I':
			coordinate[0] = 8;
			break;
		case 'J':
			coordinate[0] = 9;
			break;
		default: 
			printf("Invalid coordinate");
			break;
	}
	if (input[1] != ' ')
	{
			printf("Invalid coordinate");
	}
	switch (input[2])
	{
		case '0':
			coordinate[1] = 0;
			break;
		case '1':
			coordinate[1] = 1;
			break;
		case '2':
			coordinate[1] = 2;
			break;
		case '3':
			coordinate[1] = 3;
			break;
		case '4':
			coordinate[1] = 4;
			break;
		case '5':
			coordinate[1] = 5;
			break;
		case '6':
			coordinate[1] = 6;
			break;
		case '7':
			coordinate[1] = 7;
			break;
		case '8':
			coordinate[1] = 8;
			break;
		case '9':
			coordinate[1] = 9;
			break;
		default: 
			printf("Invalid coordinate");
			break;
	}
}

int update()
{
	int hitormiss;
	if (board[coordinate[0]][coordinate[1]] != 0) //0 is empty space, 9 is a shot, other numbers are ships
	{
		
		hitormiss = 1;//hit
	}
	else
	{
		hitormiss = 0;
	}
	board[coordinate[0]][coordinate[1]] = 9;
	return hitormiss;
}

void display(int hit)
{
	if(hit == 0)
	{
		printf("You have missed the shot. Get better.\n");
	}
	else
	{
		printf("You have hit your target. Keep it up.\n");
	}
    
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int main()
{
	srand(time(NULL));
	char input[3];
	initialize();
	while (input[0] != 'z')
	{
		printf("Enter a coordinate A-J, 0-9 (ex. D 7), z to stop : \n");
		scanf("%[^\n]%*c", input);
		inp(input);
		display(update());
		
	}
	teardown();
}
