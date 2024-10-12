/*
*	Данная программа представляет собой клиента,
*	который отправляет сообщение на сервер.
*	Передача сообщения происходит через raw
*	сокет, поэтому программа самостоятельно
*	формирует udp-заголовок, чтобы получатель
*	(сервер) мог корректно прочитать сообщение.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP "127.0.0.1" //ip получателя
#define PORT 8888 //порт получателя
#define SIZE 256

//структура заголовка udp
struct udphdr {
	unsigned short source_port; //порт отправителя
	unsigned short destination_port; //порт получателя
	unsigned short lenght; //длина заголовок + данные
	unsigned short check_sum; //контрольная сумма
};

int main()
{
	int sockfd;
	struct sockaddr_in dest_addr;
	char message[SIZE] = "Test text";
	char packet[SIZE];

	//создание raw сокета
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sockfd < 0)
	{
		perror("[Клиент] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	dest_addr.sin_addr.s_addr = inet_addr(IP);

	//заполнение заголовка udp
	struct udphdr *udp_header = (struct udphdr *)packet;
	udp_header->source_port = htons(12345);
	udp_header->destination_port = htons(PORT);
	udp_header->lenght = htons(sizeof(struct udphdr) + strlen(message));
	udp_header->check_sum = 0;

	//копирование сообщения в пакет после заголовка udp
	strcpy(packet + sizeof(struct udphdr), message);

	//отправка пакета
	if (sendto(sockfd, packet, sizeof(struct udphdr) + strlen(message), 0, 
				(struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
	{
		perror("[Клиент] Ошибка sendto");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Серверу отправленно сообщение: %s\n", message);

	close(sockfd); //закрытие сокета
	exit(EXIT_SUCCESS);
}