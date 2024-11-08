/*
*	В первом цикле создаётся 5 потоков,
*	во втором цикле ожидается завершение
*	потоков. В функции для поток выводится
*	индекс потока, который был получен в
*	качества параметра.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_THREADS 5

void* thread_function(void* thread_index)
{
	int tid = (long)thread_index;
	printf("Индекс потока: %d\n", tid); //выводим на экран индекс потока
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[NUMBER_THREADS];

	//создание потоков
	for (long i = 0; i<NUMBER_THREADS; i++)
	{
		pthread_create(&threads[i], NULL, thread_function, (void*)i);
	}

	//ожидание завершения потоков
	for (int i = 0; i < NUMBER_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	exit(EXIT_SUCCESS);
}