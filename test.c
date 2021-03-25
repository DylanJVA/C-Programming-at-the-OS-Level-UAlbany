#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int port;
	sscanf(argv[1], "%d", &port); 
	char adr[256];//*
	sscanf(argv[2], "%s", adr);
	int v1 = 1;
	int v2 = 10;
	int v3 = 100;
	char str[] = "ping/n";
	printf("s1 = %li, s2 = %li, s3 = %li", sizeof(v1), sizeof(v2), sizeof(v3));
	printf("Size: %d\n", (int)strlen(str));
	printf("%s\n", adr);
}