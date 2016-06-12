#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "contacts.h"

#ifndef Elements
#define Elements vertexPtr
#endif

typedef struct node
{
	vertexPtr info;
	struct node *ptr;
} Queue;

typedef Queue * QueuePtr;

QueuePtr front, rear, temp;

int count;

Elements frontelement();
void enq(Elements data);
int deq();
int isQueueEmpty();
void display();
void create();
int queuesize();

#endif