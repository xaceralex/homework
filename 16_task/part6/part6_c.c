/*
*	Эта программа получает сообщение,
*	отправленное по групповой рассылке
*	по протоколу UDP. Данная программа
*	выступает в роли клиента.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 50000
#define SERVER_IP "224.0.0.1"
#define SIZE 256

int main()
{
	int sock, recv_bytes = 0;
	struct sockaddr_in server_name;
	struct ip_mreq mreq;
	char buffer[SIZE];
	socklen_t server_name_size = sizeof(server_name);

	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		perror("[Клиент] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	memset(&server_name, 0, sizeof(server_name)); //чистим структуру
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = INADDR_ANY; //принимаем на всех интерфейсах
	server_name.sin_port = htons(PORT);

	//привязка сокета к адресу
	if (bind(sock, (struct sockaddr*)&server_name, sizeof(server_name)) < 0)
	{
		perror("[Клиент] Ошибка bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	//заполнение структуру для использвания multicast
	mreq.imr_multiaddr.s_addr = inet_addr(SERVER_IP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY); //принимаем на всех интерфейсах

	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (void*)&mreq.imr_interface, sizeof(mreq.imr_interface)) < 0)
	{
		perror("[Клиент] Ошибка setsockopt");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Начало приёма сообщений\n");

	for (int i = 0; i < 20; i++)
	{
		recv_bytes = recvfrom(sock, buffer, SIZE, 0,
						(struct sockaddr*)&server_name, &server_name_size);
		if (recv_bytes < 0)
		{
			perror("[Клиент] Ошибка recvfrom");
			close(sock);
			exit(EXIT_FAILURE);
		}
		buffer[recv_bytes] = '\0'; // Завершение строки
		printf("[Клиент] Получено байт: %d\n", recv_bytes);
		printf("[Клиент] Получена строка: %s\n", buffer);
	}

	close(sock);
	exit(EXIT_SUCCESS);
}