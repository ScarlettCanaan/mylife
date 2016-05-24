#ifndef __QUEUE_H__
#define __QUEUE_H__


	typedef struct struct_queue
	{
		int front, rear;
		int maxsize;
		int cursize;
		int* items;
	} Queue;

	Queue* Initial(Queue* queue,int maxsize) {
		queue->items = (int *)malloc(sizeof(int) * maxsize);
		queue->maxsize = maxsize;
		queue->cursize = 0;
		queue->rear = queue->front = 0;
		return queue;
	}

	int Insert(Queue* queue, int item) {
		if (!isOverflow(queue))
		{
			queue->items[queue->rear] = item;
			queue->rear = (queue->rear + 1) % queue->maxsize;
			queue->cursize++;
			return 0;
		}
		return -1;
	}

	int Remove(Queue* queue, int *val) {
		if (!isEmpty(queue))
		{
			*val = queue->items[queue->front];
			queue->front = (queue->front + 1) % queue->maxsize;
			queue->cursize--;
			return 0;
		}
		return -1;
	}

	int isEmpty(Queue* queue) {
		//return queue->front == queue->rear;
		return 0 == queue->cursize;
	}

	int isOverflow(Queue* queue) {
		//return (queue->front == (queue->rear + 1) % queue->maxsize);
		return queue->maxsize == queue->cursize;
	}

#endif