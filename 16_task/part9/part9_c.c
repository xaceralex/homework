/*
*	Данная программа выступает в роли клиента.
*	Клиент подключается к "слушающему" серверу.
*	После чего начинает взаимодействовать уже с
*	"обслуживающим" сервером. Клиент отправляет
*	"обслуживающему" серверу сообщение, затем
*	получает ответ.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777
#define BUFFER_SIZE 256
#define MESSAGE "Test text"

int main()
{
	int client_socket;
	int read_bytes = 0;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	//создание сокета
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0)
	{
		perror("[Клиент 1] socket");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP сервера
	server_addr.sin_port = htons(PORT);

	//подключение к серверу
	if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[Клиент 1] Ошибка connect");
		close(client_socket);
		exit(EXIT_FAILURE);
	}

	//обмен сообщениями с сервером в течении ~20 сек.
	for (int i = 0; i < 20; i++)
	{
		send(client_socket, MESSAGE, strlen(MESSAGE), 0); //отправка сообщения серверу

		read_bytes = recv(client_socket, buffer, sizeof(buffer), 0); //получение ответа от сервера
		if (read_bytes > 0)
		{
			printf("[Клиент 1] Сообщение от сервера: %s\n", buffer);
			printf("[Клиент 1] Получено байт: %d\n", read_bytes);
		}
		read_bytes = 0;
		sleep(1);
	}
	close(client_socket);
	exit(EXIT_SUCCESS);
}