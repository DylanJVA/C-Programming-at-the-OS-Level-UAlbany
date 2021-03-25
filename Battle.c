/* ICSI 333 Programming at the Hardware-Software Interface
 * Fall 2019
 * Tuesday 5:35pm
 * Dylan VanAllen, Partner: Yared Zeleke
 * 001394199, Partner: 001407748
 * This program models the game Battleship.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include<unistd.h>
#include<netdb.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#define ROW 10
#define COL 10
#define TOTAL_POSSIBLE_HITS 17



struct node
{

	char coordinates[3]; 
	char result[20];
	struct node *next;

};

void initialize(char ***, char ***);
void place_ship(char ***, char, int);
int position_valid(char **, int, int, int, int);
int input(char *, char *, char **, struct node **);
int promp_input(char *, char *);
void update(char, char, char ***, char **, int *, struct node **, struct node **, struct node **);
void display(char **);
void terminate(char ***, char ***, struct node **);
void insert_node (struct node **, struct node **, struct node **);
int createListenSocket(), createSendingSocket();


char ipAddress[200], port[200];
int ourSocket,listenSocket;

/* Initializes the game world.
 * char ***world_state: current state of board
 * char ***ships: current state of ship
 */

void initialize(char ***world_state, char ***ships)
{

	if (ipAddress[0] == 0)
	{
		printf("create listen socket");
		createListenSocket();
    }
	else
	{
        printf("create sending socket");
		createSendingSocket();
    }
	
	//Allocates memory for the dymanic arrays.
	*world_state = (char**)malloc(ROW * sizeof(char*));
	*ships = (char**)malloc(ROW * sizeof(char*));

	//Fills both arrays with dashes.
	for(int i = 0; i < ROW; i++)
	{
		(*world_state)[i] = (char*)malloc(COL * sizeof(char));
		(*ships)[i] = (char*)malloc(COL * sizeof(char));

		for(int j = 0; j < COL; j++)
		{
			(*world_state)[i][j] = '-';
			(*ships)[i][j] = '-';
		}
	}
	
	//Declares and initializes the length of the various ships.
	const int CARRIER = 5;
	const int BATTLESHIP = 4;
	const int CRUISER = 3;
	const int SUBMARINE = 3;
	const int DESTROYER = 2;

	place_ship(ships, 'C', CARRIER);
	place_ship(ships, 'B', BATTLESHIP);
	place_ship(ships, 'R', CRUISER);
	place_ship(ships, 'S', SUBMARINE);
	place_ship(ships, 'D', DESTROYER);

}

/* Places a ship on the ship board.
 * char ***ships: The current ship placement state
 * char ship_letter: The letter representing the ship to be placed
 * int ship_length: The length of the ship to be placed
 */
void place_ship(char ***ships, char ship_letter, int ship_length)
{
	srand((unsigned) time(NULL));
	int startx, starty, orientation;
	
	//Generate starting position variables
	do
	{
		startx = rand() % 10;
		starty = rand() % 10;
		orientation = rand() % 2;//Horizontal = 0, Vertical = 1
	}while(position_valid(*ships, startx, starty, orientation, ship_length) != 1);

	//Place the ship taking orientation into account. 
	if(orientation == 0)
	{
		for(int i = 0; i < ship_length; i++)
		{
			(*ships)[startx + i][starty] = ship_letter;
		}
	}
	else
	{
		for(int i = 0; i < ship_length; i++)
		{
			(*ships)[startx][starty + i] = ship_letter;
		}
	}
}

/* Checks if the provided parameters make a valid location for ship placement.
 * char **ships: The current ship placement state
 * int x: The horizontal coordinate of the ship to be placed
 * int y: The vertical coordinate of the ship to be placed
 * int orientation: Integer specifying the orientation of the ship to be placed
 * int ship_length: The lenght of the ship to be placed
 * Returns 1 if the position is valid, and 0 if it is not
 */
int position_valid(char **ships, int x, int y, int orientation, int ship_length)
{
	if(orientation == 0)
	{
		if((x + ship_length) > 9)
		{
			return 0;
		}
		for(int i = 0; i < ship_length; i++)
		{
			if((ships[x+i][y]) != '-')
			{
				return 0;
			}
		}
		return 1;
	}
	else
	{
		if((y + ship_length) > 9)
		{
			return 0;
		}
		for(int i = 0; i < ship_length; i++)
		{
			if(ships[x][y+i] != '-')
			{	
				return 0;
			}
		}
		return 1;
	}
}

/* Gets input from the user.
 * char *in_letter: User input letter
 * char *in_num: User input number
 * char **world_state: The current world state
 * struct node **temp: A node to hold the coordinates entered by the user
 * Returns 0 if the user enters "!" and 1 if they provide valid input
 */
int input(char *in_letter, char *in_num, char **world_state, struct node **temp)
{
	if(promp_input(in_letter, in_num) == 0)
	{
		return 0;
	}

	int x = toascii(*in_letter) - 65;
	int y = (*in_num) - '0';

	while(world_state[x][y] != '-')
	{
		printf("\nYou have already entered this postion! Try again!\n");
		
		if(promp_input(in_letter, in_num) == 0)
		{
			return 0;
		}

		x = toascii(*in_letter) - 65;
		y = (*in_num) - '0';
	}

	//Allocate memory for the node
	if (((*temp) = (struct node *)malloc(sizeof(struct node))) == NULL)
	{
		printf("Node allocation failed. \n");
		exit(1); /* Stop program */
	}
	
	//Concatenate the coordinates
	char temp_str[3] = {*in_letter, *in_num}; 
	strcpy((*temp)->coordinates, temp_str);
 	(*temp)->next = NULL;
	return 1;

}

/* Prompts the user for input and validates said input.
 * char *in_letter: User input letter
 * char *in_num: User input number
 * Returns 0 if the user enters "Z0" and 1 if they provide valid input
 */
int promp_input(char *in_letter, char *in_num)
{
	int invalid = 1;
	
	do
	{
		printf("Enter a letter A-J and number 0-9 ex. B4 - enter Z0 to end\n");
        char temp[20];
		fgets(temp, 20, stdin);
 
		if(strlen(temp) != 3)
		{

            printf("INVALID INPUT\n");
            continue;
        }
		
		*in_letter = temp[0];
		*in_num = temp[1];
		*in_letter = toupper(*in_letter);

        if((*in_letter) == 'Z' && (*in_num) == '0')
		{
			return 0;
		}

        if ((*in_letter) < 65 || (*in_letter) > 74)
		{
            printf("Invalid Letter!\n");
		}
        else if ((*in_num) < '0' || (*in_num) > '9')
		{
            printf("Invalid Number!\n");
		}
        else
		{
            invalid = 0;
		}
	}while(invalid);
	return 1;

}

/* Updates the world_state based on the input letter and number the user has entered.
 * char in_letter: User letter input
 * char in_num: User number input
 * char ***world_state: current world_state
 * char **ships: The current ship placement state
 * int *spots_hit: Integer tracking the number of spots the user has hit
 * struct node **h: The head of the linked list
 * struct node **t: The tail of the linked list
 * struct node **temp: The current node
 */

void update(char in_letter, char in_num, char ***world_state, char **ships, int *spots_hit, struct node **h, struct node **t, struct node **temp)
{
	int x = toascii(in_letter) - 65;
	int y = in_num - '0';

	switch (ships[x][y])
	{
		case 'C':
			printf("\nYou hit the carrier!");
			(*world_state)[x][y] = 'H';
			(*spots_hit)++;
			strcpy((*temp)->result, "Hit - Carrier");
			break;
		case 'B':
			printf("\nYou hit the battleship!");
			(*world_state)[x][y] = 'H';
			(*spots_hit)++;
			strcpy((*temp)->result, "Hit - Battleship");
			break;
		case 'R':
			printf("\nYou hit the cruiser!");
			(*world_state)[x][y] = 'H';
			(*spots_hit)++;
			strcpy((*temp)->result, "Hit - Cruiser");
			break;
		case 'S':
			printf("\nYou hit the submarine!");
			(*world_state)[x][y] = 'H';
			(*spots_hit)++;
			strcpy((*temp)->result, "Hit - Submarine");
			break;
		case 'D':
			printf("\nYou hit the destroyer!");
			(*world_state)[x][y] = 'H';
			(*spots_hit)++;
			strcpy((*temp)->result, "Hit - Destroyer");
			break;
		default:
			printf("\nYou missed!");
			(*world_state)[x][y] = 'M';
			strcpy((*temp)->result, "Miss");
			break;
	}
	insert_node(h, t, temp);
}

/* Displays the current state of the world.
 * char **world_state: The current world state.
 */
void display(char **world_state)
{
	char c = 'A';

	//Prints the number row above the game board
	printf("\n");

	for(int i = -1; i < COL; i++)
	{
		if(i >= 0)
		{
			printf("%d ", i);
		}
		else
		{
			printf("  ");
		}
	}
	
	printf("\n");

	//Prints the game board with the corresponding letters before each row
	for(int i = 0; i < ROW; i++)
	{
		printf("%c ", c + (i));
		for(int j = 0; j < COL; j++)
		{
			printf("%c ", world_state[i][j]);
		}
		
		printf("\n");
	}

}

/* Terminates the game world.
 * char ***world_state: The current world state
 * char ***ships: The current ship placement state
 * char **head: The head of the linked list
 */
void terminate(char ***world_state, char ***ships, struct node **head)
{
	for(int i = 0; i < ROW; i++)
	{
		free((*world_state)[i]);
		free((*ships)[i]);
	}

	free(*world_state);
	free(*ships);
	FILE *fp = fopen("move_log.txt", "w");

	if ((fp = fopen("move_log.txt", "w")) == NULL)
	{
		/* File open failed. */
		printf("Can’t open in.dat.\n"); exit(1);

	}

	int move_num = 1;
	while(*head != NULL)
	{
		fprintf(fp, "%d) Fired at %s. %s. \n", move_num, (*head)->coordinates, (*head)->result);
		move_num++;
		struct node *temp = *head;
		*head = (*head)->next;
		free(temp);
	}

	if (fclose(fp) == EOF)
	{ 
		/* Error in closing output file */
		printf("Error in closing log file.\n");
	}
}

/* Inserts the provided node into the end of the linked list.
 * struct node **h: The head of the linked list
 * struct node **t: The tail of the linked list
 * struct node **temp: The node to be inserted
 */
void insert_node (struct node **h, struct node **t, struct node **temp)
{
	if (*h == NULL)
	{
		/* List is currently empty. */
		*h = *t = *temp;
	}
	else 
	{ 
		/* The list is not empty. Use *t to add the node */
		/* at the end. */
		(*t)->next = *temp; *t = (*t)->next;
	}
}

int createSendingSocket() {
	
	
int sock, n, port, i;
struct sockaddr_in server;
struct hostent *address;
char buffer[200];
socklen_t len;
len = sizeof(server);
bzero((char *)&server, sizeof(server));
sock = socket(AF_INET, SOCK_STREAM, 0);
//int socket(domain, type, protocol)
if(sock<0)
   printf("Socket Could Not Be Created\n");
else
   printf("Socket Created Succesfully\n");

server.sin_family = AF_INET;
server.sin_port = htons(port);
address = gethostbyname(argv[1]);
bcopy((char *)address->h_addr, (char*)&server.sin_addr.s_addr, address->h_length);
//inet_pton(&server, , &server.s_addr);
n = connect(sock, (struct sockaddr*)&server, len);
if(n<0)
printf("ERROR: Connect");
   
}

int createListenSocket() 
{

	void printf(const char *msg)
	{
	   pprintf(msg);
	   exit(1);
	}
	if(argc<2)
	{
		printf("\nPort number not provided, program terminated\n");
	}
	int sock, newsock, n, port, i;
	char buffer[200];
	struct sockaddr_in address;
	socklen_t len;
	len = sizeof(address);
	bzero((char *)&address, sizeof(address));
	if((sock = socket(AF_INET, SOCK_STREAM, 0))==0)
	{
		printf("\nERROR: Socket not created\n");
	}
	port = atoi(argv[1]);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	bind(sock, (struct sockaddr*)&address, sizeof(address));
	listen(sock, 3);
	newsock = accept(sock, (struct sockaddr *)&address, &len);
	if(newsock<0)
	{
		printf("ERROR: Accept");
	}
	return newsock;
	/*
	bzero(buffer, 200);
	n = read(newsock, buffer, 200);
	if(n<0)
	printf("ERROR: READING");
	printf("\nClient: %s", buffer);
	*/
		
}

int main(int argc, char **argv)
{

	if (argc != 3 && argc != 2)
	{ 
		printf("usage: battleship [ipaddress] port\n");
		return; 
	}

	if (argc == 3) 
	{
        // if there are two command line arguments, where
        // first is the ipaddress and 
        // second is the port, then we initialize 
        // the server side in initialization() function
		strcpy(ipAddress,argv[1]);
		strcpy(port,argv[2]);
	}

	else 
	{
        // if there is only one command line argument, 
        // then we initialize the client side in initialization
		memset(ipAddress,0,200);
		strcpy(port,argv[1]);
	}

	char **world_state;				//Dynamic array modeling the state of the world.
	char **ships;					//Dynamic array modeling the placement of ships on the board.
	char input_letter = 'A';		//Variable to hold the user's letter input.
	char input_number = '0';		//Variable to hold the user's number input.
	int spots_hit = 0;				//Variable to keep track of how many spots the player has hit.
	int moves = 0;					//Variable to track the number of moves the player has made.
	struct node *head, *tail, *temp;//Nodes for linked list
	head = tail = NULL;

	//Initializes the world_state and ship placement.
	int newsock = initialize(&world_state, &ships);

	do
	{	
		display_state(state, board);
        //add code below to send our move to the other player
        n = write(, buffer, strlen(buffer));
		if(n<0)
		   printf("ERROR: WRITING");
		//add code to receive the state of our move from the other player
        bzero(buffer, 200);
		n = read(newsock, buffer, 200);
		if(n<0)
		{
		   printf("ERROR: READING");
		}
		update(buffer[0], buffer[1],&world_state, ships, &spots_hit, &head, &tail, &temp);
		printf("Opponent shot at: %s", buffer);
		bzero(buffer, 200);
		fgets(buffer, 200, stdin);
		n = write(newsock, buffer, strlen(buffer));
		if(n<0)
		   printf("ERROR: WRITING");
		printf("Client shot at %s", buffer); 
		i = strncmp(buffer,"Z0", 2);
		if(i==0)
		{
			break;
		}
		
	}while(1);
	close(newsock);
	close(sock);
	return;
		//add code to store our moves (letter, number, and result) into linked list

		struct move theirMove;
        //add code below to receive theirMove from the other player

        //modify the update_state function to check theirMove is HIT or MISS
		
        //and send the state back to the other player 

	} while(strcmp(state, flag));

	//Prints the location of the ships
	printf("\nThis is the placement of ships for this game:\n");
	display(ships);
	printf("\nSink all the ships to win! Enter \"!\" at anytime to quit the game.\n");
	display(world_state);

	//Main game loop. It runs until the user stops it by entering "!".
	while((spots_hit < TOTAL_POSSIBLE_HITS) && (input(&input_letter, &input_number, world_state, &temp) == 1))
	{
	
		update(input_letter, input_number, &world_state, ships, &spots_hit, &head, &tail, &temp);				//Updates the world_state
		moves++;
		if(spots_hit != 16)
		{
				printf("\nYou have %d more spots to hit!\n", TOTAL_POSSIBLE_HITS - spots_hit);
		}
		else
		{
			printf("\nYou have %d more spot to hit!\n", TOTAL_POSSIBLE_HITS - spots_hit);
		}
		display(world_state);																//Displays the world_state
	}


	//Checks if the player sunk all the ships.
	if(spots_hit == 17)
	{
		printf("\nCongratulations! You sunk all the ships in %d moves!\nHere's a cookie\n", moves);
	}
	else
	{
		printf("You quit after %d moves.\nThis is where the ships were:\n", moves);
		display(ships);
	}

	//Terminates the game
	terminate(&world_state, &ships, &head);
	printf("Press any key to exit.");
	getchar();
	return 0;
}