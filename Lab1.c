//Dylan VanAllen Lab 1
//Task 1-2
int task3();
int task4();
int task5();
int task6();
int task7();
int task8();
int task9();
int task10();
int task11();

#include <stdio.h>
int main() {
   printf("Hello, World!");
   task3();
   task4();
   task5();
   task6();
   task7();
   task8();
   task9();
   task10();
   task11();
   return 0;
}

//Task 3
int task3() {
    int number;

    printf("Enter an integer: \n");
    scanf("%d", &number);  
    
    printf("\nYou entered: %d\n", number);
    return 0;
}


//Task 4
int task4() {
    int firstNumber, secondNumber, sumOfTwoNumbers;
    
    printf("Enter two integers: \n");
    scanf("%d %d", &firstNumber, &secondNumber);

    sumOfTwoNumbers = firstNumber + secondNumber; 
    printf("\n%d + %d = %d\n", firstNumber, secondNumber, sumOfTwoNumbers);

    return 0;
}


//Task 5
int task5() {
    double firstNumber, secondNumber, product;
    printf("Enter two numbers: \n");

// Stores two floating point numbers in variable firstNumber and secondNumber respectively
    scanf("%lf %lf", &firstNumber, &secondNumber);  
 
// Performs multiplication and stores the result in variable productOfTwoNumbers
    product = firstNumber * secondNumber;  

    // Result up to 2 decimal point is displayed using %.2lf
    printf("\nProduct = %.2lf\n", product);
    
    return 0;
}


//Task 6
int task6() {
	char c;
    printf("Enter a character: \n");

    // Reads character input from the user
    scanf("%c", &c);  
    
    // %d displays the integer value of a character
    // %c displays the actual character
    printf("\nASCII value of %c = %d\n", c, c);
    return 0;

}


//Task 7
int task7() {
	int dividend, divisor, quotient, remainder;

    printf("Enter dividend: ");
    scanf("%d", &dividend);

    printf("Enter divisor: ");
    scanf("%d", &divisor);

    // Computes quotient
    quotient = dividend / divisor;

    // Computes remainder
    remainder = dividend % divisor;

    printf("Quotient = %d\n", quotient);
    printf("Remainder = %d", remainder);

    return 0;
}


//Task 8
int task8() {
	int integerType;
    float floatType;
    double doubleType;
    char charType;

    printf("Size of int: %ld bytes\n",sizeof(integerType));
    printf("Size of float: %ld bytes\n",sizeof(floatType));
    printf("Size of double: %ld bytes\n",sizeof(doubleType));
    printf("Size of char: %ld byte\n",sizeof(charType));

    return 0;

}


//Task 9
int task9() {
	int a;
    long b;
    long long c;

    double e;
    long double f;

    printf("Size of int = %ld bytes \n", sizeof(a));
    printf("Size of long = %ld bytes\n", sizeof(b));
    printf("Size of long long = %ld bytes\n", sizeof(c));

    printf("Size of double = %ld bytes\n", sizeof(e));
    printf("Size of long double = %ld bytes\n", sizeof(f));

    return 0;

}


//Task 10
int task10() {
   int i = 5;
   
   if(i == i == i) printf("Yes\n");
   else printf("No\n");
   
   return 0;


}


//Task 11
#define MX 5
int task11() {
	int i;
    for(i = 1;i < MX;i++) {
		printf("i = %d\n",i);
	}
    return 0;
}