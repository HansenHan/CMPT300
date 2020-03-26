#include <stdlib.h>
#include <semaphore.h> //act like a lock
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "bbuff.h"


void* buffer[BUFFER_SIZE];
sem_t fullbuffer;
sem_t emptybuffer;
sem_t mutex;
int index;

void bbuff_init(void){
	index = 0;
	sem_init(&fullbuffer, 0, 0);  // iniialzie semaphores
	sem_init(&emptybuffer, 0, BUFFER_SIZE);
	sem_init(&mutex, 0, 1);
}

void bbuff_blocking_insert(void* item){
	sem_wait(&emptybuffer);  // incremen 
	sem_wait(&mutex); // act like lock
	buffer[index] = item; // inseret
	index++;
	sem_post(&mutex);// release lock
	sem_post(&fullbuffer);	// decrement 
}

void* bbuff_blocking_extract(void){
	sem_wait(&fullbuffer);
	sem_wait(&mutex);
	index--;
	void* item = buffer[index];
	buffer[index] = NULL;
	sem_post(&mutex);
	sem_post(&emptybuffer); 
	return item;
}

_Bool bbuff_is_empty(void){
	return (index == 0); // return when boudnded buffer contains nothing
}

// void bbuff_free(void){
// 	free(buffer);
// }
