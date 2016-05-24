#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct linklist
{
	int value;
	struct linklist* next;
} Linklist;

typedef Linklist* LinkPtr;

LinkPtr bubble_sort(LinkPtr front)
{
	LinkPtr pHead = (LinkPtr)malloc(sizeof(Linklist));
	pHead->next = front;
	LinkPtr prev = NULL;
	LinkPtr swap = NULL;
	LinkPtr node = NULL;
	LinkPtr rear = front;
	int i = 1, j = 0;
	for (; rear->next != NULL; rear = rear->next, ++i);

	while (--i)
	{
		prev = pHead;
		node = pHead->next;
		j = i;
		while (j--)
		{
			swap = node->next;
			if (swap->value < node->value)
			{
				node->next = swap->next;
				swap->next = node;
				prev->next = swap;
				prev = swap;
			}
			else
			{
				prev = node;
				node = node->next;
			}

		}
	}
	return pHead->next;
}

LinkPtr addNode(LinkPtr front, int _value)
{
	LinkPtr ptr = (LinkPtr)malloc(sizeof(LinkPtr));
	ptr->value = _value;
	ptr->next = NULL;
	if (front != NULL)
	{
		LinkPtr tmp = front;
		for (; front->next != NULL; front = front->next);
		front->next = ptr;
		return tmp;
	}
	else
	{
		return ptr;
	}
}

void printValue(LinkPtr front)
{
	while (front->next != NULL)
	{
		printf("%d\n", front->value);
		front = front->next;
	}
	printf("%d\n", front->value);
}

int main(int argc, char const *argv[])
{
	srand(time(0));
	LinkPtr front = NULL;
	int arg = atoi(argv[1]);
	for (int i = 0; i < arg; ++i)
	{
		front = addNode(front, rand() + 1);
	}
	printf("Source Linklist:\n");
	printValue(front);
	front = bubble_sort(front);
	printf("Sorted Linklist:\n");
	printValue(front);
	while (front->next != NULL)
	{
		if (front->next->value < front->value)
		{
			printf("FUCK sort failed!");
			return;
		}
		front = front->next;
	}
	printf("Sort success!\n");
	return 0;
}