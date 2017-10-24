#ifndef QUEUE_H
#define	QUEUE_H

#define BUFFER 5

typedef struct {
	unsigned int vals [BUFFER + 1];

	unsigned int front;			// index of first item to be removed from queue
	unsigned int back;			// index - 1 is last item in queue
} Queue;

int isFull(Queue);
int isEmpty(Queue);
void enqueue(Queue, unsigned int);
unsigned dequeue(Queue);

/*Add Global Variables Here*/
Queue execution_queue;

#endif	/* QUEUE_H */

