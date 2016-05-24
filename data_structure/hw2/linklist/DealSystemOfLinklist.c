#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

typedef struct hand
{
	int number;
	int suit;
	struct hand* next;
	struct hand* prev;
} hand;

char SUIT[4][9] = {"hearts", "clubs", "diamonds", "spades" };
char NUMBER[13] = { 'A', '2', '3', '4', '5', '6',
					'7', '8', '9', '0', 'J', 'Q', 'K' };

int InitialDeal(hand** hands) {
	srand(time(0));
	int library[52] = { 0 };
	hand* ptr = *hands;
	for (int i = 0; i < 13; ++i)
	{
		int tmp = rand() % 52;
		if (library[tmp] == 1) {
			while (library[tmp] == 1) {
				tmp = rand() % 52;
			};
		}
		hand* tmpptr = (hand*)malloc(sizeof(hand));
		tmpptr->number = (int)(tmp / 4);
		tmpptr->suit = (int)(tmp % 4);
		tmpptr->next = NULL;
		if (*hands == NULL) 
			*hands = tmpptr;
		else {
			ptr->next = tmpptr;
			tmpptr->prev = ptr;			
		}
		ptr = tmpptr;
		library[tmp] = 1;
	}
	ptr->next = NULL;
	(*hands)->prev = ptr;
}

void printCards(hand **hands) {
	if (hands == NULL) return;
	hand* ptr = *hands;
	printf("Get hands:\n");
	while (ptr != NULL) {
		printf("%s %c\n", SUIT[ptr->suit], NUMBER[ptr->number]);
		ptr = ptr->next;
	}
}

void deleteHand(hand **ptr) {
	hand* tmp = *ptr;
	(*ptr)->prev->next = (*ptr)->next;
	if ((*ptr)->next != NULL)
	{
		(*ptr)->next->prev = (*ptr)->prev;
		*ptr = (*ptr)->next;
	} else {
		*ptr = (*ptr)->prev;
	}
	free(tmp);
}

int isRoyal_Flush(hand** hands) {
	hand *ptr = *hands;
	int curSuit = ptr->suit;
	int i = 0;
	while (ptr != NULL) {
		if (ptr->number != i && ptr->suit != curSuit) return 0;
		i++;
		ptr = ptr->next;
	}
	if (i == 13) return 1;
	else return 0;
}
void Royal_Flush(hand** hands) {
	printf("Play Royal_Flush card: %s A to K!\n", (*hands)->suit);
	while (*hands != NULL) {
		hand *ptr = *hands;
		*hands = (*hands)->next;
		free(ptr);
	}
}

int isMiscellaneous_Straight_Flush(hand** hands) {
	hand *ptr = *hands;
	int i = 0;
	while (ptr != NULL) {
		if (ptr->number != i) return 0;
		i++;
		ptr = ptr->next;
	}
	if (i == 13) return 1;
	else return 0;
}
void Miscellaneous_Straight_Flush(hand** hands) {
	printf("Play Miscellaneous_Straight_Flush card:");
	while (*hands != NULL) {
		printf("%s %c\n", SUIT[(*hands)->suit], NUMBER[(*hands)->number]);
		hand *ptr = *hands;
		hands = (*hands)->next;
		free(ptr);
	}
}

int isStraight_Flush(hand** hands) {
	hand* ptr = *hands;
	while(ptr->next != NULL) {
		int curSuit = ptr->suit;
		if (ptr->number == (ptr->next->number - 1) && ptr->next->suit == curSuit)
		{
			int counter = 1;
			hand* tmp = ptr->next;
			for (int i = 0; i < 3; ++i) {
				if (tmp->next == NULL) break;
				if (tmp->number == tmp->next->number - 1 && tmp->next->suit == curSuit)
				{
					counter++;
					tmp = tmp->next;
				} else {
					break;
				}
			}
			if (counter == 4) {
				return 1;
			}
		}
		ptr = ptr->next;
	}
	return 0;
}
void Straight_Flush(hand** hands) {
	hand* ptr = *hands;
	while(ptr->next != NULL) {
		int curSuit = ptr->suit;
		if (ptr->number == (ptr->next->number - 1) && ptr->next->suit == curSuit)
		{
			int counter = 1;
			hand* tmp = ptr->next;
			for (int i = 0; i < 3; ++i) {
				if (tmp->number == tmp->next->number - 1 && tmp->next->suit == curSuit)
				{
					counter++;
					tmp = tmp->next;
				} else {
					break;
				}
			}
			if (counter == 4) {
				printf("Play Straight_Flush card:");
				for (int j = 0; j < 5; ++j)
				{
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);



printCards(hands);



				}
				return;
			}
		}
		ptr = ptr->next;
	}
}

int isFour_of_a_Kind(hand** hands) {

}
void Four_of_a_Kind(hand** hands) {
	int numberCount[13] = { 0 };
	hand *ptr = *hands;
	while (ptr != NULL) {
		numberCount[ptr->number]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 4) {
			ptr = *hands;
			while ((ptr = ptr->next) != NULL && ptr->number != i);
			printf("Play Four_of_a_Kind card:");
			for (int j = 0; j < 4; ++j)
			{
				printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
				deleteHand(&ptr);


printCards(hands);



			}
			for (int i = 0; i < 13; ++i)
			{
				if (numberCount[i] == 1)
				{
					ptr = *hands;
					while((ptr = ptr->next) != NULL && ptr->number != i);
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);


printCards(hands);
				return;
				}
			}
		}
	}
}

int isFull_House(hand** hands) {

}
void Full_House(hand** hands)  {
	int numberCount[13] = { 0 };
	hand *ptr = *hands;
	while (ptr != NULL) {
		numberCount[ptr->number]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 3) {
			ptr = *hands;
			while ((ptr = ptr->next) != NULL && ptr->number != i);
			printf("Play Full_House card:");
			for (int j = 0; j < 3; ++j)
			{
				printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
				deleteHand(&ptr);





printCards(hands);


			}
			for (int i = 0; i < 13; ++i)
			{
				if (numberCount[i] == 2)
				{
					ptr = *hands;
printCards(hands);
					while(ptr != NULL && ptr->number != i) {
						ptr = ptr->next;
					}
printCards(hands);
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);
printCards(hands);
				return;
				}
			}
		}
	}
}

int isFlush(hand** hands) {

}
void Flush(hand** hands) {
	int suitCount[4] = { 0 };
	hand* ptr = *hands;
	while(ptr != NULL) {
		suitCount[ptr->suit]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 4; ++i)
	{
		if (suitCount[i] >= 5) {
			printf("Play Flush card:");
			ptr = *hands;
			while (ptr != NULL) {
				if (ptr->suit == i)
				{
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);


printCards(hands);



				}
				ptr = ptr->next;
			}
			return;
		}
	}
}

int isStraight(hand** hands) {
	hand* ptr = *hands;
	while(ptr->next != NULL) {
		if (ptr->number == (ptr->next->number - 1))
		{
			int counter = 1;
			hand* tmp = ptr->next;
			for (int i = 0; i < 3; ++i) {
				if (tmp->next == NULL) break;
				if (tmp->number == (tmp->next->number - 1))
				{
					counter++;
					tmp = tmp->next;
				} else {
					break;
				}
			}
			if (counter == 4) {
				return 1;
			}
		}
		ptr = ptr->next;
	}
	return 0;
}
void Straight(hand** hands) {
	hand* ptr = *hands;
	while(ptr->next != NULL) {
		int curSuit = ptr->suit;
		if (ptr->number == (ptr->next->number - 1))
		{
			int counter = 1;
			hand* tmp = ptr->next;
			for (int i = 0; i < 3; ++i) {
				if (tmp->number == (tmp->next->number - 1))
				{
					counter++;
					tmp = tmp->next;
				} else {
					break;
				}
			}
			if (counter == 4) {
				printf("Play Straight card:");
				for (int j = 0; j < 5; ++j)
				{
					printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
					deleteHand(&ptr);

printCards(hands);



				}
				return;
			}
		}
		ptr = ptr->next;
	}
}

int isThree_of_a_kind(hand** hands) {

}
void Three_of_a_kind(hand** hands) {
	int numberCount[13] = { 0 };
	hand *ptr = *hands;
	while (ptr != NULL) {
		numberCount[ptr->number]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 3) {
			ptr = *hands;
			while ((ptr = ptr->next) != NULL && ptr->number != i);
			printf("Play Three_of_a_kind card:");
			for (int j = 0; j < 3; ++j)
			{
				printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
				deleteHand(&ptr);


printCards(hands);


			}
			return;
		}
	}
}

void Pair(hand** hands) {
	int numberCount[13] = { 0 };
	hand *ptr = *hands;
	while (ptr != NULL) {
		numberCount[ptr->number]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 2) {
			ptr = *hands;
			while ((ptr = ptr->next) != NULL && ptr->number != i);
			printf("Play Pair card:");
				printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
				deleteHand(&ptr);


printCards(hands);
				printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
				deleteHand(&ptr);
printCards(hands);
			}
			return;
		}
	}
}

void Single(hand** hands) {
	int numberCount[13] = { 0 };
	hand *ptr = *hands;
	while (ptr != NULL) {
		numberCount[ptr->number]++;
		ptr = ptr->next;
	}
	for (int i = 0; i < 13; ++i)
	{
		if (numberCount[i] == 1) {
			ptr = *hands;
			while ((ptr = ptr->next) != NULL && ptr->number != i);
			printf("Play Single card:");
			printf("%s %c\t", SUIT[ptr->suit], NUMBER[ptr->number]);
			deleteHand(&ptr);




printCards(hands);


		}
		return;
	}
}

int type_buffer(hand** hands) {
	printf("\n");
	int i, single=0,pair=0,triple=0,four=0,flush=0;
	int hasdiscard = 0;
	int numberCount[13] = { 0 };
	int suitCount[4] = { 0 };
	int level = 0;
	int numberDiscard = 0;
	if (*hands == NULL)
	{
		return 1;
	}
	int curSuit = (*hands)->suit;
	hand *ptr = *hands;
	while (ptr != NULL) {
			numberCount[ptr->number]++;
			suitCount[ptr->suit]++;
			ptr = ptr->next;		
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
	if (isRoyal_Flush(hands))
	{
		printf("Could play Royal Straight Flush card!\n");
		level = 10;
	printCards(hands);
	}
	else if (isMiscellaneous_Straight_Flush(hands))
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
			printCards(hands);
			break;
		case 2:
			Pair(hands);
			printCards(hands);
			break;
		case 3:
			Three_of_a_kind(hands);
			printCards(hands);
			break;
		case 4:
			Straight(hands);
			printCards(hands);
			break;
		case 5:
			Flush(hands);
			printCards(hands);
			break;
		case 6:
			Full_House(hands);
			printCards(hands);
			break;
		case 7:
			Four_of_a_Kind(hands);
			printCards(hands);
			break;
		case 8:
			Straight_Flush(hands);
			printCards(hands);
			break;
		case 9:
			Miscellaneous_Straight_Flush(hands);
			printCards(hands);
			break;
		case 10:
			Royal_Flush(hands);
			printCards(hands);
			break;
		default:
			return 1;
	}
	if (*hands == NULL)
	{
		return 1;
	}
	return 0;
}

int insertSortCards(hand** hands) {
	if (*hands == NULL) {
			return 1;
		}
	hand* first;
	hand* p;
	hand* q;
	hand* r;
	first = (*hands)->next;
	(*hands)->next = NULL;
	while (first != NULL) {
		for (p = *hands, r = first; ((p != NULL) && (p->number < r->number)); q = p, p = p->next);
		first = first->next;
		if (p == *hands)
		{
			*hands = r;
		} else {
			q->next = r;
			//r->prev = q;			
		}
		r->next = p;
		//p->prev = r;
	}
	return 0;
}