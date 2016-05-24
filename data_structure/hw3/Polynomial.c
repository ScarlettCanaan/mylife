#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

typedef struct Polynomial
{
	int power;
	int coefficient;
	struct Polynomial* next;
}Polynomial;

typedef Polynomial* PolyPtr;

void printPoly(PolyPtr poly)	//Function A:Print the representaation of such a polynomial.
{
	PolyPtr i = poly;
	for (; (i->next) != NULL; i = i->next)
	{
		printf("%dx^%d+", i->coefficient, i->power);
	}
	printf("%dx^%d\n", i->coefficient, i->power);
}

int evaluate(PolyPtr poly, int value)		//Function B:Evaluate such a polynomial for given value x.
{
	int temp = 0;
	for (PolyPtr i = poly; i != NULL; i = i->next)
	{
		temp += i->coefficient * pow(value, i->power);
	}
	return temp;
}

PolyPtr addTerm(PolyPtr poly, int _power, int _confficient)
{
	if (poly == NULL)
	{
		poly = (PolyPtr)malloc(sizeof(Polynomial));
		poly->power = _power;
		poly->coefficient = _confficient;
		poly->next = NULL;
		return poly;
	}
	else {
		PolyPtr i = poly;
		for (; i->power > _power && i->next != NULL; i = i->next);
		if (i->power == _power)
		{
			i->coefficient += _confficient;
			return poly;
		}
		else {
			PolyPtr temp = (PolyPtr)malloc(sizeof(Polynomial));
			temp->coefficient = _confficient;
			temp->power = _power;
			if (i->power > _power) {
				temp->next = i->next;
				i->next = temp;
				return poly;
			}
			else {
				if (i == poly)
				{
					poly = temp;
					temp->next = i;
					return poly;
				}
				else {
					PolyPtr ptr = poly;
					while (ptr->next != i && ptr->next != NULL) {
						ptr = ptr->next;
					}
					temp->next = i;
					ptr->next = temp;
					return poly;
				}
			}
		}
	}
}

PolyPtr operatorAdd(PolyPtr polyA, PolyPtr polyB)	//Function C:Add two such polynomials as a new polynomial.
{
	PolyPtr result = NULL;
	for (PolyPtr i = polyA; i != NULL; i = i->next)
	{
		result = addTerm(result, i->power, i->coefficient);
	}
	for (PolyPtr i = polyB; i != NULL; i = i->next)
	{
		result = addTerm(result, i->power, i->coefficient);
	}
	return result;
}

PolyPtr operatorMultiply(PolyPtr polyA, PolyPtr polyB)//Function D:Multiply two such polynomials as a new polynomial.
{
	PolyPtr result = NULL;
	for (PolyPtr i = polyA; i != NULL; i = i->next)
	{
		for (PolyPtr j = polyB; j != NULL; j = j->next)
		{
			result = addTerm(result, i->power * j->power, i->coefficient * j->coefficient);
		}
	}
	return result;
}

int main() {
	// printf("A and B are polynomial, above those polynomial will random assgined 5 times:\n");
	// srand(time(0));
	// for (int i = 0; i < 6; ++i)
	// {
	// 	PolyPtr polyA = NULL;
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	printf("A: "); printPoly(polyA);
	// 	PolyPtr polyB = NULL;
	// 	polyB = addTerm(polyB, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyB = addTerm(polyB, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyB = addTerm(polyB, rand() % 5 + 1, rand() % 5 + 1);
	// 	printf("B: "); printPoly(polyB);
	// 	PolyPtr result = NULL;
	// 	result = operatorAdd(polyA, polyB);
	// 	printf("A + B: "); printPoly(result);
	// 	free(result);
	// 	result = operatorMultiply(polyA, polyB);
	// 	printf("A * B: "); printPoly(result);
	// }
	// printf("A is a random constructed polynomial, above this polynomial will evaluate value for given random x:\n");
	// for (int i = 0; i < 3; ++i)
	// {
	// 	PolyPtr polyA = NULL;
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	polyA = addTerm(polyA, rand() % 5 + 1, rand() % 5 + 1);
	// 	printf("A: "); printPoly(polyA);
		
	// 	for (int i = 0; i < 5; ++i)
	// 	{
	// 		int x = rand() % 100;
	// 		printf("x : %d   f(x) = :%d\n", x, evaluate(polyA, x));
	// 	}
	// }
}