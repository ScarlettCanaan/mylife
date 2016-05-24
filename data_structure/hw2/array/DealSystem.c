#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct hand
{
	int number;
	int suit;
	int discard;
} hand;

char SUIT[4][9] = {"hearts", "clubs", "diamonds", "spades" };
char NUMBER[13] = { 'A', '2', '3', '4', '5', '6',
					'7', '8', '9', '0', 'J', 'Q', 'K' };

int InitialDeal(hand* hands) {
	srand(time(0));
	int library[52];
	for (int i = 0; i < 52; ++i)
	{
		library[i] = 0;
	}
	for (int i = 0; i < 13; ++i)
	{
		int tmp = rand() % 52;
		if (library[tmp] == 1) {
			while (library[tmp] == 1) {
				tmp = rand() % 52;
			};
		}
		hands[i].number = (int)(tmp / 4);
		hands[i].suit = (int)(tmp % 4);
		hands[i].discard = 0;
		library[tmp] = 1;
	}
}

int printCards(hand *hands) {
	printf("Get hands:\n");
	for (int i = 0; i < 13; ++i)
	{
		printf("%s %c\n", SUIT[hands[i].suit], NUMBER[hands[i].number]);
	}
}

int isRoyal_Flush(hand* hands) {
	int curSuit = hands[0].suit;
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 1)
		{
			return 0;
		}
		if (hands[i].number != i && hands[i].suit == curSuit)
		{
			return 0;
		}
	}
	return 1;
}
void Royal_Flush(hand* hands) {
	printf("Play Royal_Flush card: %s A to K!\n", SUIT[hands[0].suit]);
	for (int i = 0; i < 13; ++i)
	{
		hands[i].discard = 1;
	}
}

int isMiscellaneous_Straight_Flush(hand* hands) {
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].number != i)
		{
			return 0;
		}
	}
	return 1;
}
void Miscellaneous_Straight_Flush(hand* hands) {
	printf("Play Miscellaneous_Straight_Flush card:");
	for (int i = 0; i < 13; ++i)
	{
		hands[i].discard = 1;
		printf("%s %c\t", SUIT[hands[i].suit], NUMBER[hands[i].number]);
	}

}

int isStraight_Flush(hand* hands) {
	for (int i = 0; i < 13 - 5; ++i)
	{
		int counter = 0;
		for (int j = i; j < (i + 4); ++j)
		{
			int curSuit = hands[j].suit;
			if (hands[j].number == (hands[j + 1].number - 1) && hands[j].suit == curSuit && hands[j].discard == 0)
			{
				counter++;
			}
		}
		if (counter == 5)
		{
			return 1;
		}
	}
	return 0;
}
void Straight_Flush(hand* hands) {
	for (int i = 0; i < 13; ++i)
	{
		int counter = 0;
		for (int j = i; j < 13; ++j)
		{
			int curSuit = hands[j].suit;
			if (hands[j].number == (hands[j + 1].number - 1) && hands[j].suit == curSuit && hands[j].discard == 0)
			{
				counter++;
			} else {
				break;
			}
		}
		if (counter >= 5)
		{
			printf("Play Straight_Flush card:");
			for (int k = i; k < i + counter; ++k)
			{
				printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
				hands[k].discard = 1;
			}
			return;
		}

	}
}

int isFour_of_a_Kind(hand* hands) {
	int numberCount[13] = { 0 };
	int nodiscard = 0;
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
			nodiscard++;
		}
	}
	if (nodiscard >= 5)
	{
		for (int i = 0; i < 13; ++i)
		{
			if (numberCount[i] == 4)
			{
				return 1;
			}
		}
	}
	return 0;
}
void Four_of_a_Kind(hand* hands) {
	// for (int i = 0; i < 13; ++i)
	// {
	// 	int counter = 0;
	// 	for (int j = i; j < 12; ++j)
	// 	{
	// 		if (hands[j].number == hands[j + 1].number && hands[i].discard == 0)
	// 		{
	// 			counter++;
	// 			if (counter >= 4)
	// 			{
	// 				printf("Play Four_of_a_Kind card:");
	// 				for (int k = i; k < i + 4; ++k)
	// 				{
	// 					printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
	// 				}
	// 			}
	// 		} else {
	// 			break;
	// 		}
	// 	}
	// }
	int numberCount[13] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 4) {
			for (int j = 0; j < 13; ++j)
			{
				if (hands[j].number == i && hands[j].discard == 0)
				{
					printf("Play Four_of_a_Kind card:");
					for (int k = 0; k < 4; ++k)
					{
						printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
						hands[k].discard = 1;
					}
					for (int l = 0; l < 13; ++l)
					{
						if (numberCount[l] == 1) {
							for (int m = 0; m < 13; ++m)
							{
								if (hands[m].number == l && hands[m].discard == 0)
								{
									printf("%s %c\t", SUIT[hands[m].suit], NUMBER[hands[m].number]);
									hands[m].discard = 1;
									return;
								}
								
							}
						}
					}
				}
			}
		}
	}
}

int isFull_House(hand* hands) {

}
void Full_House(hand* hands)  {
	int numberCount[13] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 3) {
			for (int j = 0; j < 13; ++j)
			{
				if (hands[j].number == i && hands[j].discard == 0)
				{
					printf("Play Full_House card:");
					for (int k = 0; k < 3; ++k)
					{
						printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
						hands[k].discard = 1;
					}
					for (int l = 0; l < 13; ++l)
					{
						if (numberCount[l] == 2) {
							for (int m = 0; m < 13; ++m)
							{
								if (hands[m].number == l && hands[m].discard == 0)
								{
									printf("%s %c\t", SUIT[hands[m].suit], NUMBER[hands[m].number]);
									printf("%s %c\t", SUIT[hands[m + 1].suit], NUMBER[hands[m + 1].number]);
									hands[m].discard = 1;
									hands[m+1].discard = 1;
									return;
								}
								
							}
						}
					}
				}
			}
		}
	}
}

int isFlush(hand* hands) {

}
void Flush(hand* hands) {
	int suitCount[4] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			suitCount[hands[i].suit]++;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		if (suitCount[i] == 5) {
			for (int j = 0; j < 13; ++j)
			{
				printf("Play Flush card:");
				if (hands[j].suit == i && hands[j].discard == 0)
				{
					printf("%s %c\t", SUIT[hands[j].suit], NUMBER[hands[j].number]);
				}
			}
		}
	}
}

int isStraight(hand* hands) {
	for (int i = 0; i < 13 - 5; ++i)
	{
		int counter = 0;
		for (int j = i; j < (i + 4); ++j)
		{
			int curSuit = hands[j].suit;
			if (hands[j].number == (hands[j + 1].number - 1) && hands[j].discard == 0)
			{
				counter++;
			}
		}
		if (counter == 5)
		{
			return 1;
		}
	}
	return 0;
}
void Straight(hand* hands) {
	for (int i = 0; i < 13; ++i)
	{
		int counter = 0;
		for (int j = i; j < 13; ++j)
		{
			int curSuit = hands[j].suit;
			if (hands[j].number == (hands[j + 1].number - 1) && hands[j].discard == 0)
			{
				counter++;
			}else break;
			if (counter >= 5)
			{
				printf("Play Straight card:");
				for (int k = i; k < i + counter; ++k)
				{
					printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
					hands[k].discard = 1;
				}
				return;
			}
		}
	}
}

int isThree_of_a_kind(hand* hands) {

}
void Three_of_a_kind(hand* hands) {
	int numberCount[13] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 3) {
			for (int j = 0; j < 13; ++j)
			{
				if (hands[j].number == i && hands[j].discard == 0)
				{
					printf("Play Three_of_a_kind card:");
					for (int k = j; k < j + 3; ++k)
					{
						printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
						hands[k].discard = 1;
					}
					return;
				}
			}
		}
	}
}

void Pair(hand* hands) {
	int numberCount[13] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 2) {
			for (int j = 0; j < 13; ++j)
			{
				if (hands[j].number == i && hands[j].discard == 0)
				{
					printf("Play Pair card:");
					for (int k = j; k < j + 2; ++k)
					{
						printf("%s %c\t", SUIT[hands[k].suit], NUMBER[hands[k].number]);
						hands[k].discard = 1;
					}
					return;
				}
			}
		}
	}
}

void Single(hand* hands) {
	int numberCount[13] = { 0 };
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 1) {
			for (int j = 0; j < 13; ++j)
			{
				if (hands[j].number == i && hands[j].discard == 0)
				{
					printf("Play Single card:");
					printf("%s %c\t", SUIT[hands[j].suit], NUMBER[hands[j].number]);
					hands[j].discard = 1;
					return;
				}
			}
		}
	}
}

int type_buffer(hand* hands) {
	printf("\n");
	int i, single=0,pair=0,triple=0,four=0,flush=0;
	int isRoyal_Flush = 1;
	int isMiscellaneous_Straight_Flush = 1;
	int hasdiscard = 0;
	int numberCount[13] = { 0 };
	int suitCount[4] = { 0 };
	int level = 0;
	int numberDiscard = 0;
	if (hands == NULL)
	{
		return 1;
	}

	int curSuit = hands[i].suit;
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 0)
		{
			numberCount[hands[i].number]++;
			suitCount[hands[i].suit]++;
		}
		if (hands[i].discard == 1)
		{
			isRoyal_Flush = 0;
			isMiscellaneous_Straight_Flush = 0;

		}
		if (hands[i].suit == curSuit)
		{
			isRoyal_Flush = 0;
		} else if (hands[i].number != i) {
			isMiscellaneous_Straight_Flush = 0;
			isRoyal_Flush = 0;
		}

	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 1) single++;
		if (numberCount[i] == 2) pair++;
		if (numberCount[i] == 3) triple++;
		if (numberCount[i] == 4) four++;
	}
	for (int i = 0; i < 4; ++i)
	{
		if (suitCount[i] >= 5) flush++;
	}
	if (isRoyal_Flush)
	{
		printf("Could play Royal Straight Flush card!\n");
		level = 10;
	}
	else if (isMiscellaneous_Straight_Flush)
	{
		printf("Could play Miscellaneous Straight Flush card!\n");
		level = 9;
	}
	if (isStraight_Flush(hands))
	{
		printf("Could Play Straight Flush card!\n");
		if (level < 8) level = 8;
	}
	if (four && single)
	{
		printf("Could play Four of a Kind card %d times!\n", (four>single)?single:four);
		if (level < 7) level = 7;
	}
	if (triple && pair)
	{
		printf("Could play Full house card %d times!\n", (triple>pair)?triple:pair);
		if (level < 6) level = 6;
	}
	if (flush)
	{
		printf("Could play Flush card %d times!\n", flush);
		if (level < 5) level = 5;
	}
	if (isStraight(hands))
	{
		printf("Could play Straight card!\n");
		if (level < 4) level = 4;		
	}
	if (triple)
	{
		printf("Could play Three of a kind card %d times!\n", triple);
		if (level < 3) level = 3;
	}
	if (pair)
	{
		printf("Could play Pair card %d times!\n", pair);
		if (level < 2) level = 2;
	}
	if (single)
	{
		printf("Could play Single card %d times!\n", single);
		if (level < 1) level = 1;
	}
	switch (level) {
		case 1:
			Single(hands);
			break;
		case 2:
			Pair(hands);
			break;
		case 3:
			Three_of_a_kind(hands);
			break;
		case 4:
			Straight(hands);
			break;
		case 5:
			Flush(hands);
			break;
		case 6:
			Full_House(hands);
			break;
		case 7:
			Four_of_a_Kind(hands);
			break;
		case 8:
			Straight_Flush(hands);
			break;
		case 9:
			Miscellaneous_Straight_Flush(hands);
			break;
		case 10:
			Royal_Flush(hands);
			break;
		default:
			return 1;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (hands[i].discard == 1)
		{
			numberDiscard++;
		}
	}
	if (numberDiscard >= 13)
	{
		return 1;
	}else {
		return 0;
	}
}

int bubbleSortCards(hand* hands) {
	if (hands == NULL) {
			return 1;
		}
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13 - i - 1; j++) {
			if (hands[j + 1].number < hands[j].number)
			{
				hand temp = hands[j];
				hands[j] = hands[j + 1];
				hands[j + 1] = temp;
			}
		}
	}
	return 0;
}