#include<stdio.h>
//This header file contains declarations used in most input and output and is typically included in all C programs.
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
// The header file socket.h includes a number of definitions of structures needed for sockets.
#include<netinet/in.h>
//The header file in.h contains constants and structures needed for internet domain addresses
#include<sys/types.h>
// This header file contains definitions of a number of data types used in system calls. These types are used in the next two include files.

void error(const char* msg)
{
   perror(msg);
   exit(1);
}
int main(int argc, char * argv[])
{
if(argc<2)
error("\nIP address, Port number not provided\n");
if(argc<3)
error("\nPort number not provided\n");
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
   error("Socket Could Not Be Created\n");
else
   printf("Socket Created Succesfully\n");

port = atoi(argv[2]);
server.sin_family = AF_INET;
server.sin_port = htons(port);
address = gethostbyname(argv[1]);
bcopy((char *)address->h_addr, (char*)&server.sin_addr.s_addr, address->h_length);
//inet_pton(&server, , &server.s_addr);
n = connect(sock, (struct sockaddr*)&server, len);
if(n<0)
error("ERROR: Connect");

do{
   bzero(buffer, 200);
   fgets(buffer, 200, stdin);
   n = write(sock, buffer, strlen(buffer));
   if(n<0)
       error("ERROR: WRITING");
   printf("\nMe: %s", buffer);
   bzero(buffer, 200);
   n = read(sock, buffer, 200);
   if(n<0)
       error("ERROR: READING");
   printf("\nServer: %s", buffer);
   i = strncmp(buffer,"Bye", 3);
   if(i==0)
       break;
}while(1);
close(sock);
return 0;
}
