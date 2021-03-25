//Dylan VanAllen Lab 5 - Linked Lists
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct s_node 
{
	int value; 
	struct s_node *next;
} node;


node *insert_node (node *h, int v)// inserts new element
{   
    node *newElement = malloc(sizeof(node));
	if (newElement == NULL) 
	{
		printf("Allocation failed.\n"); exit(1);
	}
	newElement->next = NULL;
	newElement->value = v;
	if(h != NULL) 
	{
		h->next = newElement;
	} 
	return newElement;
}

void print_list(node *h) //prints the list
{
	if (h == NULL) 
	{
		printf("The list is empty.\n");
	}
	else 
	{
		printf("Values in the list are:\n"); 
		int count = 0;
		while (h != NULL) 
		{
			printf("%d, ", h->value); 
			h = h->next;
			count++;
		}
		printf("\nCount: %d\n", count);
	}
}

//task 2
node *last_occur (node *h, int x)
{
	int in_list = 0;
	node *t = h;
	node *t2 = h;
	if (t == NULL) 
	{
		printf("The list is empty.\n");
	}
	else 
	{
		while (t!=NULL)
		{
			if (t->value == x)
			{
				in_list++;
			}
			t = t->next;
		}
	}
	if (in_list == 0)
	{
		return NULL;
	}
	else
	{
		int count = 0;
		while (count != in_list)
		{
			if (t2->value == x)
			{
				count++;
			}
			t2 = t2->next;
		}
		return t2;
	}
}


//task 3
void delete_node (node *ph, int x)
{
	if (ph == NULL)
	{
		printf("The list is empty. \n");
	}
	else if (ph->value == x)
	{
		ph = ph->next;
	}
	else
	{
		while (ph != NULL)
		{
			if (ph->next->value == x)
			{
				ph->next = ph->next->next;
				break;
			}
			ph = ph->next;
		}
	}
}

int main()
{
	node *head = NULL;
	node *tail = NULL;
	srand(time(NULL));
	int x = rand() % 25;
	while(x != 8)
	{
		x = rand() % 25;
		if (x != 8 && head == NULL)
		{
			head = insert_node(head, x);
			tail = head;
		}
		else if (x != 8 && head != NULL)
		{
			tail = insert_node(tail, x);
		}
	}
	print_list(head);
	int n = rand() % 25;
	printf("Last occurence of %d: %p\n",n,last_occur(head,n));
	delete_node(head, n);
	printf("List after deleting %d\n", n);
	print_list(head);
}