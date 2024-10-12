/*
*	Эта программа выступает в роли клиента.
*	Клиент подключается к "слушающему" серверу
*	и получает от него порт "обслуживающего"
*	сервера. После этого подключается к
*	"обслуживающему" серверу и отправляем ему 
*	строку "Test text", потом получает
*	преобразованную строку. В процессе отправки
*	и получения строк, они выводятся на экран
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777
#define SIZE 256
#define MESSAGE "Test text"

int main()
{
	int sock = 0;
	struct sockaddr_in server_name;
	char buffer[SIZE] = {0};
	int *new_port = (int *)malloc(sizeof(new_port));
	int read_bytes = 0;

	//cоздаем сокет
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Клиент 1] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	//заполняем структуру
	server_name.sin_family = AF_INET;
	server_name.sin_port = htons(PORT);

	//преобразование ip в бинарный формат
	if (inet_pton(AF_INET, "127.0.0.1", &server_name.sin_addr) <= 0)
	{
		printf("[Клиент 1] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}

	//подключение к "слушающему" серверу
	if (connect(sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("[Клиент 1] Ошибка connect\n");
		exit(EXIT_FAILURE);
	}

	read_bytes = recv(sock, new_port, sizeof(new_port), 0);
	printf("[Клиент 1] От сервера получен новый порт: %d\n", *new_port);

	sleep(1); //ожидание, чтобы сервер успел создать новый поток для клиента

	close(sock);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Клиент 1] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	//заполняем структуру
	server_name.sin_port = htons(*new_port);
	server_name.sin_family = AF_INET;

	//преобразование ip в бинарный формат
	if (inet_pton(AF_INET, "127.0.0.1", &server_name.sin_addr) <= 0)
	{
		perror("[Клиент 1] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}

	//подключение к "обслуживающему" серверу
	if (connect(sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		perror("[Клиент 1] Ошибка connect");
		exit(EXIT_FAILURE);
	}

	send(sock, MESSAGE, strlen(MESSAGE), 0); //отправка данных "обслуживающему" серверу

	//получение преобразованных данных и печать их на экран
	read_bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
	if (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		printf("[Клиент 1] Сообщение от сервера: %s\n", buffer);
		printf("[Клиент 1] Получено байт: %d\n", read_bytes);
	}

	close(sock); //закрываем сокет
	exit(EXIT_SUCCESS);
}