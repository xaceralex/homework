/*
*	Эта программа отправляет сообщение
*	по широковещательной рассылке по
*	протоколу UDP. Данная программа
*	выступает в роли сервера.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777
#define BROADCAST_IP "255.255.255.255"
#define MESSAGE "Test text"

int main()
{
	int sock;
	struct sockaddr_in server_name;
	int flag = 1;
	int send_bytes = 0;

	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		perror("[Сервер] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	//разрешение для широковещательной рассылки
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0)
	{
		perror("[Сервер] Ошибка setsockopt\n");
		close(sock);
		exit(EXIT_FAILURE);
	}

	memset(&server_name, 0, sizeof(server_name)); //чистим структуру
	server_name.sin_family = AF_INET;
	server_name.sin_port = htons(PORT);
	server_name.sin_addr.s_addr = inet_addr(BROADCAST_IP);

	//отправка сообщения
	send_bytes = sendto(sock, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr*)&server_name, sizeof(server_name));
	if (send_bytes < 0)
	{
		perror("[Сервер] Ошибка sendto\n");
		close(sock);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("[Сервер] Было отправленно байт: %d\n", send_bytes);
		close(sock);
		exit(EXIT_SUCCESS);
	}

	close(sock);
	exit(EXIT_SUCCESS);
}