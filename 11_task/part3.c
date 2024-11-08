/*
*	Функция «customer» определяем работу
*	условных покупателей. Функция «loader»
*	определяем работу условных грузчиков.
*	В функции «main» создаются
*	потоки-покупатели и потоки-грузчики,
*	определяются объёмы запасов магазинов.
*	Также функция «main» ожидает завершение
*	потоков-покупателей, завершает работу
*	потоков-грузчиков, высвобождает ресурсы
*	для мьютексов.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_SHOPS 5 //количество магазинов
#define NUMBER_CUSTOMERS 3 //количество покупателей
#define NUMBER_LOADERS 4 //количество грузчиков
#define MAX_NEED 100000 //максимальная потребность
#define MAX_STORE 10000 //максимальный запас магазина
#define LOAD_AMOUNT 5000 //объём товара для грузчика

int shops[NUMBER_SHOPS]; //объём магазинов их "запасы"
pthread_mutex_t shop_mutexes[NUMBER_SHOPS]; //мьютексы для каждого магазина
int satisfied_customers = 0; //количество покупателей закрывшие "потребность"

void* customer(void* arg) //функция для потоков покупателей
{
	int customer_id = *(int*)arg; //фиксация номера покупателя
	int need = rand() % MAX_NEED; //определение случайного объёма "потребности"
	int buy; //покупка или "корзина покупателя"

	while (need > 0 && satisfied_customers < NUMBER_CUSTOMERS) //входим в цикл если есть потребность и 
	{
		for (int i = 0; i < NUMBER_SHOPS; i++) //проход по всем магазинам
		{
			if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) //если магазин свободен, то "блокируем" его
			{
				printf(" Покупатель %d зашёл в магазин %d, объём магазина: %d, потребность покупателя: %d\n", customer_id, i, shops[i], need);

				if (shops[i] > 0) //если магазин не пуст, то покупатель совершает покупку
				{
					buy = (need < shops[i]) ? need : shops[i]; //определение объёма покупки исходя из текущей потр. и "запаса" магазина
					shops[i] -= buy; //уменьшаем "запас" магазина на объём покупки покупателя
					need -= buy; //уменьшаем размер потрбности на объём покупки покупателя
					printf(" Покупатель %d закупился на %d в магазине %d текущая потребность %d\n", customer_id, buy, i, need);
				}

				printf(" Покупатель %d засыпает на 2 сек.\n\n", customer_id);
				sleep(2); //покупатель засыпает на 2 сек.

				pthread_mutex_unlock(&shop_mutexes[i]); //разблокируем магазин
				break; //если не получилось войти в магазин пробуем снова возможно какой-то уже освободился
			}
		}
	}

	if (need <= 0) //если потребность удовлетворена
	{
		satisfied_customers++; //увеличиваем количество покупателей удовлетворивших потребность
		printf(" Покупатель %d удовлетворил потребность\n\n", customer_id);
	}

	return NULL;
}

void* loader(void* arg) //функция для потоков грузчиков
{
	int loader_id = *(int*)arg; //фиксация номера грузчика

	while (satisfied_customers < NUMBER_CUSTOMERS) //если ещё есть покупатели с потребностями
	{
		for (int i = 0; i < NUMBER_SHOPS; i++) //проход по всем магазинам
		{
			if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) //если магазин свободен, то "блокируем" его
			{
				printf(" Грузчик %d вошёл в магазин %d с запасом %d\n", loader_id, i, shops[i]);

				shops[i] += LOAD_AMOUNT; //пополнение запасов магазина
				printf(" Грузчик %d пополнил запас на %d для магазина %d, текущий запас магазина: %d\n", loader_id, LOAD_AMOUNT, i, shops[i]);

				printf(" Грузчик %d засыпает на 1 сек.\n\n", loader_id);
				sleep(1);

				pthread_mutex_unlock(&shop_mutexes[i]); //разблокируем магазин
				break; //если не получилось войти в магазин пробуем снова возможно какой-то уже освободился
			}
		}
	}

	return NULL;
}

int main()
{
	srand(time(NULL)); //обновление генератора случайных чисел

	//инициализация магазинов и мьютексов для них
	for (int i = 0; i < NUMBER_SHOPS; i++)
	{
		shops[i] = rand() % MAX_STORE;
		printf(" Магазин %d его запасы %d\n", i, shops[i]);
		pthread_mutex_init(&shop_mutexes[i], NULL);
	}

	printf("\n");

	pthread_t customers[NUMBER_CUSTOMERS];
	pthread_t loaders[NUMBER_LOADERS];

	int customer_ids[NUMBER_CUSTOMERS]; 
	int loader_ids[NUMBER_LOADERS];

	//инициализация покупателей
	for (int i = 0; i < NUMBER_CUSTOMERS; i++)
	{
		customer_ids[i] = i + 1;
		pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
	}

	//инициализация грузчиков
	for (int i = 0; i < NUMBER_LOADERS; i++)
	{
		loader_ids[i] = i + 1;
		pthread_create(&loaders[i], NULL, loader, &loader_ids[i]);
	}

	//ожидание завершения потоков покупателей
	for (int i = 0; i < NUMBER_CUSTOMERS; i++)
	{
		pthread_join(customers[i], NULL);
	}

	//завершение работы грузчиков
	for (int i = 0; i < NUMBER_LOADERS; i++)
	{
		pthread_cancel(loaders[i]);
	}

	//высвобождение ресурсов мьютексов
	for (int i = 0; i < NUMBER_SHOPS; i++)
	{
		pthread_mutex_destroy(&shop_mutexes[i]);
	}

	printf("Все покупатели завершили свои покупки.\n");

	exit(EXIT_SUCCESS);
}