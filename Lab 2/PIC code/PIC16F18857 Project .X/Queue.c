#include "Queue.h"
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

int isFull(Queue data) {
	return (
		((data.back + 1) % (BUFFER + 1))
			== data.front);
}
int isEmpty(Queue data) {
	return (data.back == data.front);
}
void enqueue(Queue data, unsigned int item) {
	if (!isFull(data)) {
		data.vals[data.back] = item;
		data.back = (data.back + 1) % (BUFFER + 1);
	}
}
unsigned int dequeue(Queue data) {
	unsigned int ret;
	if (!isEmpty(data)) {
		ret = data.vals[data.front];
		data.front = (data.front + 1) % (BUFFER + 1);
	}
	return ret;
}
