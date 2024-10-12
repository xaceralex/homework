/*
*	Данная программа выступает в роли клиента.
*	Клиент отправляет сообщение серверу 
*	используя протокол udp.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777

int main()
{
	int sock;
	struct sockaddr_in serv_addr;
	char *message = "Test text UDP";

	//создание сокета
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("[Сервер UDP] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//преобразование строки в сетевой адрес
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		perror("[Сервер UDP] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}

	sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)); //отправка серверу сообщения
	printf("[Сервер UDP] Серверу отправленно сообщение: %s\n", message);

	close(sock);
	exit(EXIT_SUCCESS);
}