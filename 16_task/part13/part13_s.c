/*
*	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PCKT_LEN 8192

int main()
{
	int sock;
	int n;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[PCKT_LEN];

	//создание сокета
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
	{
		perror("[Сервер] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//заполнение данных сервера
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(63123);
	server_addr.sin_addr.s_addr = INADDR_ANY; // Принимаем данные от любого адреса

	//привязка сокета к адресу
	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[Сервер] Ошибка bind");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		//получение сообщения
		n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &addr_len);
		if (n < 0)
		{
			perror("[Сервер] Ошибка recvfrom");
			exit(EXIT_FAILURE);
		}

		buffer[n] = '\0';//закрываем строку

		printf("[Сервер] Получено сообщение: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
	}

	close(sock); // Закрытие сокета
	exit(EXIT_SUCCESS);
}
