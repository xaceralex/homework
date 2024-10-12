/*
*	Эта программа отправляет сообщение
*	используя групповую рассылку
*	протоколу UDP. Данная программа
*	выступает в роли сервера.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 50000
#define SERVER_IP "224.0.0.1"
#define MESSAGE "Test text"

int main()
{
	int sock, send_bytes = 0;
	struct sockaddr_in server_name;

	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		perror("[Сервер] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	memset(&server_name, 0, sizeof(server_name)); //чистим структуру
	server_name.sin_family = AF_INET;
	server_name.sin_port = htons(PORT);
	server_name.sin_addr.s_addr = inet_addr(SERVER_IP);

	//будем отправлять 20 сообщений с промежутком в 1 сек.
	//т.е. "трансляция" будет длиться 20 секунд
	for (int i = 0; i < 20; i++)
	{
		send_bytes = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
				(struct sockaddr*)&server_name, sizeof(server_name));
		if (send_bytes < 0)
		{
			perror("[Сервер] Ошибка sendto");
			close(sock);
			exit(EXIT_FAILURE);
		}
		printf("[Сервер] Отправленно сообщение: %s\n", MESSAGE);
		sleep(1); //ожидание 1 сек
	}

	close(sock);
	exit(EXIT_SUCCESS);
}