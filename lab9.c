#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
//Dylan VanAllen Lab9 Networking
//Somewhere im overwriting into memory but i dont know where
void server(int port)
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET/**/, SOCK_STREAM/**/, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket\n");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = port;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	printf("Address: %u\n",INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		printf("ERROR on binding\n");
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) 
	{
		printf("ERROR on accept\n");
	}
	bzero(buffer,256);
	n = read(newsockfd,buffer,sizeof(buffer));
	if (n < 0)
	{
		printf("ERROR reading from socket\n");
	}
	printf("%s\n",buffer);
	n = write(newsockfd,"pong\n",6);
	if (n < 0)
	{
		printf("ERROR writing to socket\n");
	}
}

void client(int port, char adr[256])
{
	int sock = 0, n; //*
    struct sockaddr_in serv_addr; 
    char *ping = "ping"; 
    char buffer[256]; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, adr, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
    }  
    n = write(sock,"ping\n",6);//*
	if (n < 0)
	{
		printf("ERROR writing to socket\n");
	}
    printf("%s\n",buffer); 
}

int main(int argc, char *argv[])
{
	int port;
	sscanf(argv[1], "%d", &port); 
	char adr[256];
	sscanf(argv[2], "%s", adr);
	if (argc < 2)
	{
		client(port, adr);
	}
	else
	{
		server(port);
	}
	return 0;
}