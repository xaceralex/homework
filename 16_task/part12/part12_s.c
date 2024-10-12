/*
*	Программа представляет собой сервер,
*	который получает сообщение от клиента
*	и выводит его на экран. Для получения
*	сообщения используются протоколы
*	AF_INET и UDP.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define SIZE 256

int main()
{
	int sockfd;
	struct sockaddr_in my_addr, client_addr;
	char buffer[SIZE];
	socklen_t addr_len = sizeof(client_addr);
	int recv_len;

	//создание сокета
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("[Сервер] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
	{
		perror("[Сервер] Ошибка bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Идёт приём на порту: %d\n", PORT);

	//цикл в котором будем получать пакеты
	while (1)
	{
		recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
					(struct sockaddr *)&client_addr, &addr_len);
		if (recv_len < 0)
		{
			perror("[Сервер] Ошибка recvfrom");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		printf("[Сервер] Получено сообщение: %s\n", buffer);
	}

	close(sockfd); //закрытие сокета
	exit(EXIT_SUCCESS);
}