/* Project 4 - Battleship
 * Dylan VanAllen, Yared Zeleke
 * Create a Battleship game that can be played through a TCP socket
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#define SIZE 10
#define localhost "127.0.0.1"


// Structure to hold each move made
struct move{
    char letter;
    int number;
    char state[20];
    char ship[20];
    int player;
    int win;
    struct move *next;
};

char ipAddress[200], port[200];
int ourSocket,listenSocket;

//Creates a board for the player
void generateShip(char** board, int size, char letter) {
	int noGood = 1;
	int orientation , row, col;
    int curRow, curCol;
	while (noGood) {
		orientation = random() %2;
		if (orientation == 0) { // Horizontal
			row = random() % 10;
			col = random() % (10 - size);//So it doesnt go off the board
		} else {
			row = random() % (10 - size);//So it doesnt go off the board
			col = random() % 10;
		}
		int noObstructions = 1;
		for (int i=0;i<size;i++) {
			curRow = row, curCol = col;
			if (orientation == 0) { // Horizontal
				curCol += i;
			} else {
				curRow += i;
			}
			if (board[curRow][curCol] != '-') 
                noObstructions = 0;	
		}
		if (noObstructions == 0) 
            continue;
		noGood = 0;
	}

	for (int i=0;i<size;i++) {
		curCol = col, curRow = row;
		if (orientation == 0) { // Horizontal
			curCol += i;
		} else {
			curRow += i;
		}
		board[curRow][curCol] = letter;
	}
}
//Signal Handler for signals through the socket
void sigchld_handler(int s)
{
	(void)s; 
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6 and return in correct type:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//Client side - create socket and connect to known ip
void createSendingSocket() {
	int numbytes;
    char buf[30];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo(ipAddress, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((ourSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        
        if (connect(ourSocket, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(ourSocket);
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(2);
    }
    
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("Connecting to %s ...\n", s);
    freeaddrinfo(servinfo); 
}


void createListenSocket() {
	//Server Side, create socket without ip

	int new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP	

	int backlog = 10;
    
    if ((rv = getaddrinfo(localhost, port, &hints, &servinfo)) != 0) //Using 127.0.0.1 as ip for testing, Can change as needed. 
	{
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((listenSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(listenSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(listenSocket);
            perror("server: bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo); // all done with this structure
    
	if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(listenSocket, backlog) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
	printf("Server Address: %s\n", localhost);
    printf("Waiting for opponent to connect...\n");

    sin_size = sizeof their_addr;
    new_fd = accept(listenSocket, (struct sockaddr *)&their_addr, &sin_size);
    
    if (new_fd == -1) {
        perror("accept");
    }
    
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    
    printf("Got connection from %s\n", s);

    close(listenSocket);

    listenSocket = new_fd;
}


//Initializes the server/client connection and creates boards
char** initialization(char ***ourMoveBoard){
	if (ipAddress[0] == 0){
        /*add function call of create listen socket*/
        createListenSocket();
    }
	else{
        /*add function call of create sending socket*/
        createSendingSocket();
    }

	int i, j;
	char **board = (char**)malloc(sizeof(char*)*SIZE);
    *ourMoveBoard = (char**)malloc(sizeof(char*)*SIZE);
	for (i = 0; i < SIZE; i++){
		board[i] = (char*)malloc(sizeof(char)*SIZE);
        (*ourMoveBoard)[i] = (char*)malloc(sizeof(char)*SIZE);
	}
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			board[i][j] = '-';
			(*ourMoveBoard)[i][j] = '-';
		}
	}

	generateShip(board, 2,'D');
	generateShip(board, 3,'S');
	generateShip(board, 3,'C');
	generateShip(board, 4,'B');
	generateShip(board, 5,'R');
    return board;
}


//Inserts the move of type struct move into a linked list
void insert_move(struct move **head, struct move **tail,struct move *temp){
     if (*head == NULL){
        /* List is currently empty. */
        *head = *tail = temp;
    }
    else{
        (*tail)->next = temp;
        *tail = (*tail)->next;
    }
}
//updates the state of your world after the opponents shot
void update_state(char* state, char ** board, struct move** head,struct move** tail, struct move** temp){
	int row, i, j;
    char letter = (*temp)->letter;
    int col = (*temp)->number;
    row = letter % 65;
    if(board[row][col] == '-'){
        strcpy(state, "MISS");
        strcpy((*temp)->state, "MISS");
        strcpy((*temp)->ship, "NONE");
        board[row][col]='O';
    }
    else{
        strcpy(state, "HIT");
        strcpy((*temp)->state, "HIT!");
        switch (board[row][col]){
            case 'C':  strcpy((*temp)->ship, "Crusier"); break;
            case 'R':  strcpy((*temp)->ship, "Carrier"); break;
            case 'B':  strcpy((*temp)->ship, "Battleship"); break;
            case 'S':  strcpy((*temp)->ship, "Submarine"); break;
            case 'D':  strcpy((*temp)->ship, "Destroyer"); break;
        }
        board[row][col]='X';
    }

    int counter = 0;
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            if(board[i][j] == '-' || board[i][j] == 'X' || board[i][j] == 'O')
                counter += 1;
        }
    }
    if(counter == SIZE * SIZE)
        strcpy(state, "GAME OVER!");

    insert_move(head,tail,*temp);
}
//Sends your move of type struct move to opponents board
void update_our_move_board(char ***ourMoveBoard, struct move *ourMove){
    int row, i, j;
    char letter = ourMove->letter;
    int col = ourMove->number;
    row = letter % 65;

    if(!strcmp(ourMove->state, "HIT!")){
        char ship = ourMove->ship[2];

        switch (ship)
        {
        case 'u':
            (*ourMoveBoard)[row][col] = 'C';
            break;
        case 'r':
            (*ourMoveBoard)[row][col] = 'R';
            break;
        case 't':
            (*ourMoveBoard)[row][col] = 'B';
            break;
        case 'b':
            (*ourMoveBoard)[row][col] = 'S';
            break;
        case 's':
            (*ourMoveBoard)[row][col] = 'D';
            break;
        }
        
    }else
        (*ourMoveBoard)[row][col] = 'O';
}
//struct to accept input
struct move* accept_input(char **ourMoveBoard){
    char letter;
    int number;
    bool flag = true;
    do{
        printf("Enter a letter A-J and number 0-9 (ex. B4). Enter Z0 to quit.\n");
        int size = scanf(" %c%d", &letter, &number);
        if(size != 2){
            printf("INVALID INPUT\n");
            continue;
        }
        letter = toupper(letter);

        if(letter == 'Z' && number == 0)
            break;
        
        int row = letter % 65;

        if(letter < 65 || letter > 74)
            printf("INVALID INPUT\n");
        else if(number <0 || number >9)
            printf("INVALID INPUT\n");
        else if(ourMoveBoard[row][number] != '-')
            printf("You have already entered this move!\n");
        else
            flag = false;
	}while(flag);

    struct move *temp;
    temp = (struct move *)malloc(sizeof(struct move));
    temp->letter = letter;
    temp->number = number;
    temp->win = 0;
    return temp;
}
//displays the board
void display_state(char** board, char** ourMoveBoard){
	int i, j;
    printf("\n**** Previos Shots ****           **** Your Board ****\n");

	printf("  0 1 2 3 4 5 6 7 8 9               0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < SIZE; i++){
        printf("%c ", 65+i);
		for (j = 0; j < SIZE; j++){
			printf("%c ", ourMoveBoard[i][j]);
		}
        
        printf("            %c ", 65+i);
		for (j = 0; j < SIZE; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}
//tears down board
int teardown(char ** board, struct move* head, struct move* tail){
	int i;
    struct move* temp;
	for(i = 0; i < SIZE; i++)
		free(board[i]);
	free(board);
	FILE *fptr;

    if(ipAddress[0] == 0)
        fptr = fopen("server_log.txt", "w");
    else
        fptr = fopen("clietn_log.txt", "w");

    if (fptr == NULL)
    {
        exit(-1);
    }


    if (head==NULL){
        fprintf(fptr, "No moves were made.");
    }
    else{
        while (head != NULL){
            if(head->player == 1){
                switch (head->state[0]){
                    case 'H':
                        fprintf(fptr, "You fired at %c%d %s %s \n", head->letter, head->number, head->state, head->ship);
                        break;
                    
                    default:
                        fprintf(fptr, "You fired at %c%d %s \n", head->letter, head->number, head->state);
                        break;
                }
            }else{
                switch (head->state[0]){
                    case 'H':
                        fprintf(fptr, "Your opponent fired at %c%d %s %s \n", head->letter, head->number, head->state, head->ship);
                        break;
                    
                    default:
                        fprintf(fptr, "Your opponent fired at %c%d %s \n", head->letter, head->number, head->state);
                        break;
                }
            }
            temp = head;
            head = head->next;
            free(temp);
        }

        if(tail->win == 1)
            fprintf(fptr, "You sunk all of your opponent's ships. You won the game!");
        else
            fprintf(fptr, "Your opponent sunk all of your ships. You lost the game!");
    }


    fclose(fptr);
    close(listenSocket);
    close(ourSocket);
	return 0;
}




void main(int argc, char **argv) {
	if (argc != 3 && argc != 2) { printf ("usage: battleship [ipaddress] port\n"); return; }
	if (argc == 3) {
        // if there are two command line arguments, where
        // first is the ipaddress and 
        // second is the port, then we initialize 
        // the client side in initialization() function
		strcpy(ipAddress,argv[1]);
		strcpy(port,argv[2]);
	}
	else {
        // if there is only one command line argument, 
        // then we initialize the server side in initialization
        // function
		memset(ipAddress,0,200);
		strcpy(port,argv[1]);
	}

	srand(time(NULL));
	char **board, **ourMoveBoard;
    char state[] = "GAME START";
    char flag[] = "GAME OVER!";

    struct move *head, *tail, *ourMove, *theirMove;
    head = tail = NULL;
    
    /*modify the initialization function */
	board = initialization(&ourMoveBoard);

    printf("\n\nYour guesses are on the board to the left.\n");
    printf("Your ships and your opponent's guesses will appear on the board to the right.\n");


    char buff[20];
    char ourMoveStateAndShip[40] = {};
    char theirMoveStateAndShip[40] = {};

	do{
        if(ipAddress[0] != 0){
            //You are the client
		    
            display_state(board, ourMoveBoard);
            
            //make move
		    ourMove = accept_input(ourMoveBoard);
            ourMove->player = 1;
            char num[2];

            sprintf(num, "%d", ourMove->number);

            buff[0] = ourMove->letter;
            buff[1] = ' ';
            buff[2] = num[0];

            //send move
            send(ourSocket, buff, sizeof(buff), 0);

            if(ourMove->letter == 'Z' && ourMove->number == 0){
                printf("\nYou quit the game.\n");
                break;
            }
            
            //wait for state
            recv(ourSocket, ourMoveStateAndShip, sizeof(ourMoveStateAndShip), 0);

            
            if(ourMoveStateAndShip[0] == 'W'){
                //You sunk all of your opponent's ships
                strcpy(state, "GAME OVER!");
                strcpy(ourMoveStateAndShip, ourMoveStateAndShip + 1);
            }

            //store move
            strcpy(ourMove->state, strtok(ourMoveStateAndShip, " "));
            strcpy(ourMove->ship, strtok(NULL, " "));
            
            strcpy(ourMoveStateAndShip, "");

            if(ourMove->state[0] == 'H')
                printf("\nYou hit their %s!\n", ourMove->ship);
            else
                printf("\nYou missed!\n");
            
            //Update our move board
            update_our_move_board(&ourMoveBoard, ourMove);

            display_state(board, ourMoveBoard);
            
            if(strcmp(state, flag) == 0){
                printf("\nYou sunk all of your opponent's ships! You win!\n");
                ourMove->win = 1;
                insert_move(&head, &tail, ourMove);
                break;
            }

            //Insert the move in the list
            insert_move(&head, &tail, ourMove);


            //wait for move
            printf("\nWaiting for your opponent to make a move...\n");
            fflush(stdout);
            
            recv(ourSocket, buff, sizeof(buff), 0);
            theirMove = (struct move *)malloc(sizeof(struct move));
            char letter = buff[0];
            int number = buff[2] - '0';

            if(letter == 'Z' && number == 0){
                printf("\nYour opponent quit the game!\n");
                break;
            }
            
            printf("\nTheir move was: %c%d\n", letter, number);

            theirMove->letter = letter;
            theirMove->number = number;
            theirMove->player = 2;
            update_state(state, board, &head, &tail, &theirMove);

            if(theirMove->state[0] == 'H')
                printf("They hit your %s!\n", theirMove->ship);
            else
                printf("They missed!\n");

            if(strcmp(state, flag) == 0){
                //Your opponent sunk all your ships
                strcpy(theirMoveStateAndShip, "W");
            }

            //send state
            strcat(theirMoveStateAndShip, theirMove->state);
            strcat(theirMoveStateAndShip, " ");
            strcat(theirMoveStateAndShip, theirMove->ship);
            send(ourSocket, theirMoveStateAndShip, sizeof(theirMoveStateAndShip), 0);

            strcpy(theirMoveStateAndShip, "");

            if(strcmp(state, flag) == 0){
		        display_state(board, ourMoveBoard);
                printf("\nYour opponent sunk all of your ships! You lost!\n");
            }
        }else{
            //you are server
            //wait for move
            display_state(board, ourMoveBoard);
            
            printf("\nWaiting for your opponent to make a move...\n");
            fflush(stdout);
            
            //Receive move
            recv(listenSocket, buff, sizeof(buff), 0);
            
            theirMove = (struct move *)malloc(sizeof(struct move));
            char letter = buff[0];
            int number = buff[2] - '0';

            if(letter == 'Z' && number == 0){
                printf("\nYour opponent quit the game!\n");
                break;
            }

            printf("\nTheir move was: %c%d\n", letter, number);

            theirMove->letter = letter;
            theirMove->number = number;
            theirMove->player = 2;
            update_state(state, board, &head, &tail, &theirMove);

            if(theirMove->state[0] == 'H')
                printf("They hit your %s!\n", theirMove->ship);
            else
                printf("They missed!\n");
            
            if(strcmp(state, flag) == 0){
                //Your opponent sunk all your ships
                strcpy(theirMoveStateAndShip, "W");
            }
            
            //send state
            strcat(theirMoveStateAndShip, theirMove->state);
            strcat(theirMoveStateAndShip, " ");
            strcat(theirMoveStateAndShip, theirMove->ship);
            send(listenSocket, theirMoveStateAndShip, sizeof(theirMoveStateAndShip), 0);

            strcpy(theirMoveStateAndShip, "");

		    display_state(board, ourMoveBoard);

            if(strcmp(state, flag) == 0){
                printf("\nYour opponent sunk all of your ships! You lost!\n");
                break;
            }

            //make move
            ourMove = accept_input(ourMoveBoard);
            ourMove->player = 1;
            char num[2];
            
            sprintf(num, "%d", ourMove->number);

            buff[0] = ourMove->letter;
            buff[1] = ' ';
            buff[2] = num[0];

            //send move
            send(listenSocket, buff, sizeof(buff), 0);

            if(ourMove->letter == 'Z' && ourMove->number == 0){
                printf("\nYou quit the game.\n");
                break;
            }
            
            //wait for state
            recv(listenSocket, ourMoveStateAndShip, sizeof(ourMoveStateAndShip), 0);
            
            if(ourMoveStateAndShip[0] == 'W'){
                //You sunk all of your opponent's ships
                strcpy(state, "GAME OVER!");
                strcpy(ourMoveStateAndShip, ourMoveStateAndShip + 1);
            }

            //store move
            strcpy(ourMove->state, strtok(ourMoveStateAndShip, " "));
            strcpy(ourMove->ship, strtok(NULL, " "));

            strcpy(ourMoveStateAndShip, "");

            if(ourMove->state[0] == 'H')
                printf("\nYou hit their %s!\n", ourMove->ship);
            else
                printf("\nYou missed!\n");

            //Update our move board
            update_our_move_board(&ourMoveBoard, ourMove);

            if(strcmp(state, flag) == 0){
                display_state(board, ourMoveBoard);
                printf("\nYou sunk all of your opponent's ships! You win!\n");
                ourMove->win = 1;
            }

            //Insert the move in the list
            insert_move(&head, &tail, ourMove);
        }
	} while(strcmp(state, flag));

	teardown(board, head, tail);
	exit(0);
}