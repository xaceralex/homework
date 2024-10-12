/*
*	Эта программа выступает в роли "слушающего"
*	сервера. Сервер заранее создаёт несколько
*	потоков с уникальными портами в качестве
*	обслуживающих серверов. После этого получает
*	заявки от клиентов и передаёт им порты
*	серверов, которые не заняты в данный момент,
*	отмечает сервера как "занятые".
*	Обслуживающий сервер обрабатывает заявку
*	клиента и после этого сигнализирует, что
*	сервер теперь свободен.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 7777
#define SIZE 256
#define SIZE_ARR 5

typedef struct {
	int new_port;
	bool busy;
} server_serving;

void *handle_client(void* arg)
{
	pthread_t thread_id = pthread_self();
	int opt = 1;
	int server_fd, client_fd;
	struct sockaddr_in server_name;
	int server_name_size = sizeof(server_name);
	int read_bytes = 0;
	char buffer[SIZE];
	server_serving *serv_str = (server_serving*)arg;

	//cоздание сокета
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("[Сервер new(%lu)]\n", (unsigned long)thread_id);
		perror("[Сервер new] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//привязываем сокет к порту
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		printf("[Сервер new(%lu)]\n", (unsigned long)thread_id);
		perror("[Сервер new] Ошибка setsockopt");
		exit(EXIT_FAILURE);
	}

	//заполняем структуру
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = INADDR_ANY;
	server_name.sin_port = htons(serv_str->new_port);

	//привязываем сокет к адресу
	if (bind(server_fd, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("[Сервер new(%lu)] Ошибка bind\n", (unsigned long)thread_id);
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		//ожидаем подключения
		if (listen(server_fd, 1) < 0)
		{
			printf("[Сервер new(%lu)] Ошибка listen\n", (unsigned long)thread_id);
			exit(EXIT_FAILURE);
		}

		printf("[Сервер new(%lu)] Идёт приём на порту: %d\n", (unsigned long)thread_id, serv_str->new_port);

		if ((client_fd = accept(server_fd, (struct sockaddr *)&server_name, (socklen_t*)&server_name_size)) < 0)
		{
			printf("[Сервер new(%lu)] Ошибка accept\n", (unsigned long)thread_id);
			exit(EXIT_FAILURE);
		}

		while ((read_bytes = read(client_fd, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[read_bytes] = '\0'; //закрываем строку

			printf("[Сервер new(%lu)] Получено от клиента: %s\n", (unsigned long)thread_id, buffer); //выводим полученное сообщение на экран
			printf("[Сервер new(%lu)] Получено байт: %d\n",(unsigned long)thread_id, read_bytes);

			buffer[read_bytes] = '+'; //изменяем строку
			buffer[read_bytes+1] = '\0'; //закрываем строку
			read_bytes++;
			send(client_fd, buffer, read_bytes, 0); //отправляем клиенту изменённую строку
		}
		sleep(5); //имитация работы
		serv_str->busy = 0;
		printf("[Сервер new(%lu)] Мы поменяли флаг в структуре: %d\n", (unsigned long)thread_id, serv_str->busy);
	}
	close(client_fd); //закрываем сокет
	return NULL;
}

int main()
{
	int server_fd, new_socket;
	struct sockaddr_in server_name;
	int opt = 1;
	int server_name_size = sizeof(server_name);

	server_serving serv_arr[SIZE_ARR];

	//cоздание сокета
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("[Сервер] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//привязываем сокет к порту
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("[Сервер] Ошибка setsockopt");
		exit(EXIT_FAILURE);
	}

	//заполняем структуру
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = INADDR_ANY;
	server_name.sin_port = htons(PORT);

	//привязываем сокет к адресу
	if (bind(server_fd, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		perror("[Сервер] Ошибка bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("[Сервер] Ошибка listen");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Идёт приём на порту: %d\n", PORT);

	//заполнение массива структур
	for (int i = 0; i < SIZE_ARR; i++)
	{
		//serv_arr[i].new_port = (int *)malloc(sizeof(int));
		serv_arr[i].new_port = PORT+1+i;
		serv_arr[i].busy = 0;
	}

	//создание 5 обслуживающих серверов
	for (int i = 0; i < SIZE_ARR; i++)
	{
		pthread_t thread_id;
		if (pthread_create(&thread_id, NULL, handle_client, (void*) &serv_arr[i]) != 0)
		{
			perror("[Сервер] Ошибка pthread_create");
			exit(EXIT_FAILURE);
		}
		pthread_detach(thread_id);
	}

	for (int i = 1; i < 20; i++)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&server_name, (socklen_t*)&server_name_size)) < 0)
		{
			perror("[Сервер] Ошибка accept");
			exit(EXIT_FAILURE);
		}

		int *new_sock = malloc(sizeof(int));
		*new_sock = new_socket;

		for (int j = 0; i < SIZE_ARR; j++)
		{
			if (serv_arr[j].busy == 0)
			{
				printf("[Сервер] Отправляем клиенту новый порт: %d\n", serv_arr[j].new_port);
				if (send(new_socket, &serv_arr[j].new_port, sizeof(serv_arr[j].new_port), 0) == -1) //отправляем клиенту новый порт для подключения
				{
					printf("[Сервер] Ошибка send");
					exit(EXIT_FAILURE);
				}
				serv_arr[j].busy = 1;
				break;
			}
		}
	}
	exit(EXIT_SUCCESS);
}