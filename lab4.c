//Dylan VanAllen Lab4 read/write file word by word#include <stdio.h>int main(){	FILE *fp = fopen("record.txt", "r");	int ch = getc(fp); 	//return value of getc is integer 	while (ch != EOF) 	//or use feof(fp) function	{		if (ch == ' ')//checks if whitespace character is at pointer		{			printf("\n");			ch = getc(fp);		}		putchar(ch); //display the char 		ch = getc(fp);	}	printf("\n");			fclose(fp); return 0;}