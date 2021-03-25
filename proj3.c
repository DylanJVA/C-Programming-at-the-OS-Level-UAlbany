//Dylan VanAllen Battleship
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define numships 5
char input[3];

typedef struct s_node 
{
	int xcoord; 
	int ycoord;
	char ship[20];
	struct s_node *next;
} node;

typedef struct initial_board_space
{
	int xcoord;
	int ycoord;
	char ship[20];
	int hit; //0 if not hit, 1 if hit
	struct initial_board_space *next;
} initial_board_space;

	
void teardown(node **h)
{
	node **t = h;
	while (*t)
	{
		free(&(*t)->next);
		t=&(*t)->next;
		if (&(*t)->ship == NULL)
		{
			free(t);
		}
	}
}

initial_board_space *initialize()
{
	int sizeofship;
	int is_vertical = 0;
	int randx = 0;
	int randy = 0;
	int intersection = 0;
	initial_board_space * first_space;
	do
	{
		intersection = 0;
		for (int row=0;row<10;row++)
		{
			for (int col=0;col<10;col++)
			{
				initial_board_space *empty_board = malloc(sizeof(initial_board_space));
				if (empty_board == NULL) 
				{
					printf("Allocation failed.\n"); exit(1);
				}
				empty_board->xcoord=col;
				empty_board->ycoord=row;
				strcpy(empty_board->ship, "Empty");
				empty_board->hit = 0;//no hit
				if (col == 0 && row == 0)
				{
					first_space = empty_board;
				}
				empty_board=empty_board->next;
			}
		}
		
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
				
				for (int row = 0; row < randy; row++)//to iterate through linked list
				{
					for (int col = 0; col < randx; col++)
					{
						first_space = first_space->next;
					}
				}
				if (strcmp(first_space->ship, "Empty") != 0)
				{
					intersection = 1;
				}
				switch (n)
				{
				case 1:
					strcpy(first_space->ship, "Carrier (5)");
					break;
				case 2:
					strcpy(first_space->ship, "Battleship (4)");
					break;
				case 3:
					strcpy(first_space->ship, "Destroyer (3)");
					break;
				case 4:
					strcpy(first_space->ship, "Submarine (3)");
					break;
				case 5:
					strcpy(first_space->ship, "Patrol Boat(2)");
					break;
				default:
					break;
				}
			}
			else
			{
				randx = rand() % (10-sizeofship)+1;
				randy = rand() % 10;
				for (int row = 0; row < randy; row++)//to iterate through linked list
				{
					for (int col = 0; col < randx; col++)
					{
						first_space = first_space->next;
					}
				}
				if (strcmp(first_space->ship, "Empty") != 0)
				{
					intersection = 1;
				}
				switch (n)
				{
				case 1:
					strcpy(first_space->ship, "Carrier (5)");
					break;
				case 2:
					strcpy(first_space->ship, "Battleship (4)");
					break;
				case 3:
					strcpy(first_space->ship, "Destroyer (3)");
					break;
				case 4:
					strcpy(first_space->ship, "Submarine (3)");
					break;
				case 5:
					strcpy(first_space->ship, "Patrol Boat(2)");
					break;
				default:
					break;
				}
			}
		}
		
	} while(intersection == 1);
	return first_space;
}
					
node *inp(char input[3], initial_board_space * initboard)
{
	node *s = malloc(sizeof(node));
	if (s == NULL) 
	{
		printf("Allocation failed.\n"); exit(1);
	}
	switch (toupper(input[0]))
	{
		case 'A':
			s->xcoord = 0;
			break;
		case 'B':
			s->xcoord = 1;
			break;
		case 'C':
			s->xcoord = 2;
			break;
		case 'D':
			s->xcoord = 3;
			break;
		case 'E':
			s->xcoord = 4;
			break;
		case 'F':
			s->xcoord = 5;
			break;
		case 'G':
			s->xcoord = 6;
			break;
		case 'H':
			s->xcoord = 7;
			break;
		case 'I':
			s->xcoord = 8;
			break;
		case 'J':
			s->xcoord = 9;
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
			s->ycoord = 0;
			break;
		case '1':
			s->ycoord = 1;
			break;
		case '2':
			s->ycoord = 2;
			break;
		case '3':
			s->ycoord = 3;
			break;
		case '4':
			s->ycoord = 4;
			break;
		case '5':
			s->ycoord = 5;
			break;
		case '6':
			s->ycoord = 6;
			break;
		case '7':
			s->ycoord = 7;
			break;
		case '8':
			s->ycoord = 8;
			break;
		case '9':
			s->ycoord = 9;
			break;
		default: 
			printf("Invalid coordinate");
			break;
	}
	for (int row = 0; row < 10; row++)//to iterate through linked list
	{
		for (int col = 0; col < 10; col++)
		{
			initboard = initboard->next;
		}
	}
	strcpy(s->ship, initboard->ship);
	
	FILE *fp;
	fp = fopen("log.txt", "w");
	putc("You fired at: %[^\n]%*c", s->hit, input);
	fclose(fp);
	return s;
	
}

initial_board_space ** update(initial_board_space **head)
{
	initial_board_space **t = head;
	for(int row=0;row<(&(*t)->ycoord);row++)
	{
		for(int col=0;col<(&(*t)->xcoord);col++)
		{
			t=&(*t)->next;
		}
	}
	&(*t)->hit = 1;
	return head9;
}

void display(node **head)
{
	node **move = head;
	int hitcounter = 0;
	for (int row = 0; row < &(*move)->xcoord; row++)//to iterate through linked list
	{
		for (int col = 0; col < &(*move)->ycoord; col++)
		{
			printf("%d ", move->hit)
			if (move->hit == 1)
			{
				hitcounter++;
			}
			move = move->next;
		}
	}
	printf("\n");
	if (hitcounter == 17)//num hits needed to win
	{
		printf("You have sunk all the ships\n");
		exit(1);
	}
int main()
{
	node *initial_board = initialize();
	node ** board = * initial_board;
	node *log;
	srand(time(NULL));
	while (input[0] != 'z')
	{
		printf("Enter a coordinate A-J, 0-9 (ex. D 7), z to stop : \n");
		scanf("%[^\n]%*c", input);
		board = *update(board);
		board = board->next;
		display(board);
	}
	teardown(board);
}