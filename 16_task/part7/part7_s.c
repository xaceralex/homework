/*
*	Эта программа выступает в роли "слушающего"
*	сервера, она получает запросы от клиентов,
*	отправляет им новый порт для подключения,
*	после этого создаёт новый поток с отправленным
*	клиенту новым портом. После этого, созданный поток
*	выступать уже в роли "обслуживающего" сервера
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 7777
#define SIZE 256

void *handle_client(void* arg)
{
	pthread_t thread_id = pthread_self();
	int port_new = *((int*)arg);
	int opt = 1;
	int server_fd, client_fd;
	struct sockaddr_in server_name;
	int server_name_size = sizeof(server_name);
	int read_bytes = 0;
	char buffer[SIZE];

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
	server_name.sin_port = htons(port_new);

	//привязываем сокет к адресу
	if (bind(server_fd, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("[Сервер new(%lu)]\n", (unsigned long)thread_id);
		perror("[Сервер new] Ошибка bind");
		exit(EXIT_FAILURE);
	}

	//ожидаем подключения
	if (listen(server_fd, 1) < 0)
	{
		printf("[Сервер new(%lu)]\n", (unsigned long)thread_id);
		perror("[Сервер new] Ошибка listen");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер new(%lu)] Идёт приём на порту: %d\n", (unsigned long)thread_id, port_new);

	if ((client_fd = accept(server_fd, (struct sockaddr *)&server_name, (socklen_t*)&server_name_size)) < 0)
	{
		printf("[Сервер new(%lu)]\n", (unsigned long)thread_id);
		perror("[Сервер new] Ошибка accept");
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
	close(client_fd); //закрываем сокет
	return NULL;
}

int main()
{
	int server_fd, new_socket;
	struct sockaddr_in server_name;
	int opt = 1;
	int server_name_size = sizeof(server_name);
	int *new_port = (int *)malloc(sizeof(int));
	*new_port = 7778; //порт для потоков

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

	for (int i = 1; i < 20; i++)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&server_name, (socklen_t*)&server_name_size)) < 0)
		{
			perror("[Сервер] Ошибка accept");
			exit(EXIT_FAILURE);
		}

		printf("[Сервер] Отправляем клиенту новый порт: %d\n", *new_port);

		int *new_sock = malloc(sizeof(int));
		*new_sock = new_socket;

		send(new_socket, new_port, sizeof(new_port), 0); //отправляем клиенту новый порт для подключения

		pthread_t thread_id;

		//создание потока для клиента
		if (pthread_create(&thread_id, NULL, handle_client, (void*) new_port) != 0)
		{
			perror("[Сервер] Ошибка pthread_create");
			exit(EXIT_FAILURE);
		}
		pthread_detach(thread_id);
		sleep(1);
		(*new_port)++; //изменяем порт для последующего потока
	}
	exit(EXIT_SUCCESS);
}