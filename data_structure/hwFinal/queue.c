#include "queue.h"
#include <stdio.h>
#include <malloc.h>

/* Create an empty queue */
void create()
{
	count = 0;
	front = rear = NULL;
}

/* Returns queue size */
int queuesize()
{
	return count;
}

/* Enqueing the queue */
void enq(Elements data)
{
	if (rear == NULL)
	{
		rear = (struct node *)malloc(1 * sizeof(Queue));
		rear->ptr = NULL;
		rear->info = data;
		front = rear;
	}
	else
	{
		temp = (struct node *)malloc(1 * sizeof(Queue));
		rear->ptr = temp;
		temp->info = data;
		temp->ptr = NULL;

		rear = temp;
	}
	count++;
}

/* Dequeing the queue */
int deq()
{
	QueuePtr front1 = front;

	if (front1 == NULL)
	{
		return 0;
	}
	else
		if (front1->ptr != NULL)
		{
			front1 = front1->ptr;
			free(front);
			front = front1;
			return 1;
		}
		else
		{
			free(front);
			front = NULL;
			rear = NULL;
			return 1;
		}
	count--;
}

/* Returns the front element of queue */
Elements frontelement()
{
	if ((front != NULL) && (rear != NULL))
		return(front->info);
	else
		return NULL;
}

/* Display if queue is empty or not */
int isQueueEmpty()
{
	if ((front == NULL) && (rear == NULL))
		return 1;
	else
		return 0;
}
