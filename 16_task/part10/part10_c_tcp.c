/*
*	Данная программа выступает в роли клиента.
*	Клиент подключается к серверу и отправляет
*	ему сообщение используя протокол tcp.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777

int main()
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char *message = "Test text TCP";

	//создание сокета
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("[Сервер TCP] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//преобразование строки в сетевой адрес
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		perror("[Сервер TCP] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}

	//подключение к серверу
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("[Сервер TCP] Ошибка connect\n");
		exit(EXIT_FAILURE);
	}

	send(sock, message, strlen(message), 0); //отправка серверу сообщения
	printf("[Сервер TCP] Серверу отправленно сообщение: %s\n", message);

	close(sock);
	exit(EXIT_SUCCESS);
}