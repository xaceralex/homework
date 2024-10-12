/*
*	Программа представляет собой сниффер,
*	который перехватывает все пакеты с
*	udp протоколом. После получения пакета,
*	его содержимое выводится на экран.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 65536

//функция для вывода только печатных символов
void print_ascii(unsigned char *buf, int len)
{
	for (int i = 0; i < len; i++)
	{
		//проверка находится ли символ в диапазоне печатных символов
		if (buf[i] >= 32 && buf[i] <= 126)
		{
			printf("%c", buf[i]); //если символ печатный
		}
		else
		{
			printf("."); //если символ не печатный
		}
	}
	printf("\n"); //разрыв между выводами
}

int main()
{
	int sockfd;
	int data_size; //количество байт в пакете
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);
	unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);

	//создание raw сокета
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sockfd < 0)
	{
		perror("[Сниффер] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//захват udp-трафика будет в бесконечном цикле
	while (1)
	{
		data_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t*)&saddr_len);
		if (data_size < 0)
		{
			perror("[Сниффер] Ошибка recvfrom");
			exit(EXIT_FAILURE);
		}

		printf("[Сниффер] Получено байт: %d\n", data_size);
		print_ascii(buffer, data_size);
	}

	//закрытие сокетов
	close(sockfd);
	free(buffer);

	exit(EXIT_SUCCESS);
}