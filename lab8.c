//Dylan VanAllen
//Lab 8 Processes, Pipes
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

//(a-i) create 2 global pipes
int fd1[2];
int fd2[2];
//(b) print ping value
void ping()
{
	int var = 0;
	char temp[10];
	int tempint;
	sscanf(temp, "%d", &tempint);
	while (var < 100 || tempint <  100)
	{
		printf("ping-{%d}\n", var);
		var++;
		write(fd1[1],(void*)&var,sizeof((void*)&var));
		read(fd2[0],temp,4);
		/*I've tracked the problem to here
		maybe because im reading from a global pipe?
		I tried to fix it but I don't know how*/
		sscanf(temp, "%d", &tempint);
	}	
	exit(0);
}
//d) create an exit function for signal handler
void quit()
{
	printf("pong quitting");
	raise(SIGUSR1);
	exit(0);
}
//(c) set up a signal handler
void signal_handler()
{
	signal(SIGUSR1,quit);
	char temp[10];
	int tempint;
	while(1)
	{
		read(fd1[0], temp, 4);
		sscanf(temp, "%d", &tempint);
		tempint++;
		sprintf(temp, "%d", tempint);
		write(fd2[1], temp, 4);
	}
}
//(a)
int main(void)
{
	//(a-ii)create 2 processes
	fork();
	if (pipe(fd1))
	{
		printf("Pipe failed");
	}
	if (pipe(fd2))
	{
		printf("Pipe failed");
	}
	ping();
	
	//(a-iii)send sigusr1 signal to c process
	signal_handler();
	return 0;
}
