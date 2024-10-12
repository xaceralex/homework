/*
*	Эта программа получает сообщение
*	по широковещательной рассылке по
*	протоколу UDP. Данная программа
*	выступает в роли клиента.
*	Первым запускается клиент
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7777
#define SIZE 256

int main()
{
	int sock;
	struct sockaddr_in server_name;
	char buffer[SIZE];
	int recv_bytes;
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

		recv_bytes = recvfrom(sock, buffer, SIZE, 0,
						(struct sockaddr*)&server_name, &recv_bytes);
		if (recv_bytes < 0)
		{
			perror("[Клиент] Ошибка recvfrom");
			close(sock);
			exit(EXIT_FAILURE);
		}
		buffer[recv_bytes] = '\0'; //завершение строки
		printf("[Клиент] Получено байт: %d\n", recv_bytes);
		printf("[Клиент] Получена строка: %s\n", buffer);

	close(sock);
	exit(EXIT_SUCCESS);
}