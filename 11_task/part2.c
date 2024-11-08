/*
*	В программе есть общий счётчик и есть два
*	потока, каждый из которого инкрементируют
*	счётчик 100 раз. Чтобы организовать корректную
*	работу потоков, используется мьютекс. Как
*	итог — инрементация счётчика потоками
*	происходит последовательно, а не одновременно,
*	соответственно, это обеспечивает
*	корректность итогового значения счётчика.
*	В первом цикле создаётся 5 потоков,
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMBER_ITERATIONS 100

pthread_mutex_t mutex;
int counter = 0; //общий счётчик

void* thread_function(void* arg)
{
	for (int i = 0; i < NUMBER_ITERATIONS; i++)
	{
		pthread_mutex_lock(&mutex); //блокировка мьютекса
		counter++; //инкрементация счетчика
		pthread_mutex_unlock(&mutex); //разблокировка мьютекса
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread1, thread2;

	pthread_mutex_init(&mutex, NULL); //инициализация мьютекса

	//создание потоков
	pthread_create(&thread1, NULL, thread_function, NULL);
	pthread_create(&thread2, NULL, thread_function, NULL);

	//ожидание завершения потоков
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Значение счётчика: %d\n", counter); //вывод значения счётчика

	pthread_mutex_destroy(&mutex); //высвобождение ресурсов мьютекса

	exit(EXIT_SUCCESS);
}