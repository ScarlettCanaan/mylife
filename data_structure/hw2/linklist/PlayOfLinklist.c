#include "DealSystemOfLinklist.c"

int main(int argc, char const *argv[])
{
	hand* hands = NULL;
	InitialDeal(&hands);
	insertSortCards(&hands);
	printCards(&hands);
	while(!type_buffer(&hands)) {
		printf("I have did a deal!!!!!!!\n");
		printCards(&hands);
	}
	printf("\n");
	return 0;
}