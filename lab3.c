#include <string.h>
#include <stdio.h>
//Dylan VanAllen Lab 2

void task1()
{
	char toCount[100];
	int	countLower,countUpper;
	int	counter;

	//assign all counters to zero
	countLower=countUpper=0;

	printf("Enter a string: ");
	fgets(toCount, sizeof toCount, stdin);

	for(counter=0; counter < sizeof toCount ;counter++){

		if(toCount[counter]>='A' && toCount[counter]<='Z')
			countUpper++;
		else if(toCount[counter]>='a' && toCount[counter]<='z')
			countLower++;	
	}

	printf("Total Upper case characters: %d, Lower Case characters: %d",countUpper,countLower);
}

void task2()
{
    char s1[100], s2[100], i, j;
    printf("Enter first string: ");
    scanf("%s", s1);
    printf("Enter second string: ");
    scanf("%s", s2);
    // store length of s1 in i
    for(i = 0; s1[i] != '\0'; ++i);
    for(j = 0; s2[j] != '\0'; ++j, ++i)
    {
        s1[i] = s2[j];
    }
    s1[i] = '\0';
    printf("After concatenation: %s", s1);
}

void task3() {
  char str[20], *pt;
  int i = 0;
  printf("Enter Any string [below 20 chars] : ");
  fgets(str, sizeof str, stdin);
  pt = str;
  while (*pt != '\0') {
    i++;
    pt++;
  }
  printf("Length of String : %d", i);

}

int main()
{
	task1();
	task2();
	task3();
}