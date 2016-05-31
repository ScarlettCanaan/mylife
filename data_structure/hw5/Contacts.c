#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct contacts
{
	char* Name;
	char* phoneNumber;
	struct contacts* Next;
}Contacts;

typedef Contacts* ContPtr;

ContPtr addNode(ContPtr contPtr, char* _name, char* _phoneNumber)
{
	ContPtr ptr = (ContPtr)malloc(sizeof(Contacts));
	ptr->Name = _name;
	ptr->phoneNumber = _phoneNumber;
	if (contPtr == NULL)
	{
		contPtr = ptr;
	}
	else
	{
		ptr->Next = contPtr;
		contPtr = ptr;
	}
	return contPtr;
}

ContPtr deleteNodeOnName(ContPtr contPtr, char* _name)
{
	ContPtr ptr = contPtr;
	ContPtr prev = ptr;
	while (ptr != NULL)
	{
		if (strcmp(ptr->Name, _name) == 0)
		{
			if (ptr == contPtr)
			{
				ContPtr temp = ptr;
				contPtr = contPtr->Next;
				ptr = contPtr;
				free(temp);
			}
			else
			{
				ContPtr temp = ptr;
				prev->Next = ptr->Next;
				ptr = ptr->Next;
				free(temp);
			}
		}
		else
		{
			prev = ptr;
			ptr = ptr->Next;
		}
	}
	return contPtr;
}

ContPtr searchOnName(ContPtr contPtr, char* _name)
{
	ContPtr ptr = contPtr;
	ContPtr prev = ptr;
	while(ptr != NULL)
	{
		if (strcmp(ptr->Name, _name) == 0)
		{
			if (ptr == contPtr)
			{
				return contPtr;
			}
			else
			{
				ContPtr temp = ptr;
				prev->Next = ptr->Next;
				temp->Next = contPtr;
				return temp;
			}
		}
		else
		{
			prev = ptr;
			ptr = ptr->Next;
		}
	}
	printf("Name %s didn't on this Contact!\n", _name);
	return contPtr;
}

void printContact(ContPtr contPtr)
{
	if (contPtr == NULL)
	{
		printf("Empty!\n");
		return;		
	}
	else
	{
		while(contPtr != NULL)
		{
			printf("Name:%s Phone:%s\n", contPtr->Name, contPtr->phoneNumber);
			contPtr=contPtr->Next;
		}
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	ContPtr ptr = NULL;
	ptr = addNode(ptr, "Ada", "123456");
	ptr = addNode(ptr, "Linux", "234567");
	ptr = addNode(ptr, "Turing", "345678");
	ptr = addNode(ptr, "Shannon", "456789");
	printf("Print Contacts:\n");
	printContact(ptr);
	printf("Add Neumann 567890....\n");
	ptr = addNode(ptr, "Neumann", "567890");
	printf("Print Contacts:\n");
	printContact(ptr);
	printf("Delete Shannon....\n");
	ptr = deleteNodeOnName(ptr, "Shannon");
	printf("Print Contacts:\n");
	printContact(ptr);
	printf("Search Turing:\n");
	ptr = searchOnName(ptr, "Turing");
	printf("Name:%s Phone:%s\n", ptr->Name, ptr->phoneNumber);
	printf("Print Contacts:\n");
	printContact(ptr);
	return 0;
}