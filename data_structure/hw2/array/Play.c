#include "DealSystem.c"

int main(int argc, char const *argv[])
{
	hand hands[13];
	InitialDeal(hands);
	printCards(hands);
	bubbleSortCards(hands);
	while(!type_buffer(hands));
	printf("\n");
	return 0;
}