#include<stdio.h>
//This header file contains declarations used in most input and output and is typically included in all C programs.
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
// The header file socket.h includes a number of definitions of structures needed for sockets.
#include<netinet/in.h>
//The header file in.h contains constants and structures needed for internet domain addresses
#include<sys/types.h>
//This header file contains definitions of a number of data types used in system calls. These types are used in the next two include files.
int main(int argc, char *argv[])
{

	void error(const char *msg)
	{
	   perror(msg);
	   exit(1);
	}
	if(argc<2)
	{
		error("\nPort number not provided, program terminated\n");
	}
	int sock, newsock, n, port, i;
	char buffer[200];
	struct sockaddr_in address;
	socklen_t len;
	len = sizeof(address);
	bzero((char *)&address, sizeof(address));
	if((sock = socket(AF_INET, SOCK_STREAM, 0))==0)
	{
		error("\nERROR: Socket not created\n");
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
		error("ERROR: Accept");
	}
	/*
	bzero(buffer, 200);
	n = read(newsock, buffer, 200);
	if(n<0)
	error("ERROR: READING");
	printf("\nClient: %s", buffer);
	*/
	do
	{
	   bzero(buffer, 200);
	   n = read(newsock, buffer, 200);
	   if(n<0)
	   {
		   error("ERROR: READING");
	   }
	   printf("\nClient: %s", buffer);
	   bzero(buffer, 200);
	   fgets(buffer, 200, stdin);
	   n = write(newsock, buffer, strlen(buffer));
	   if(n<0)
		   error("ERROR: WRITING");
	   printf("\nMe: %s",buffer);
	   i = strncmp(buffer,"Bye", 3);
	   if(i==0)
	   break;
	}while(1);
	close(newsock);
	close(sock);
	return 0;
}