#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7766
#define STRING_SIZE 256

int main()
{
	int sockfd; //сокет
	char buff[STRING_SIZE];
	struct sockaddr_in server_addr; //end-point сервера

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("[Клиент]Ошибка команды socket");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Адрес сервера (localhost)

	strcpy(buff, "Test_text");
	sendto(sockfd, &buff, strlen(buff), 0, 
			(const struct sockaddr *) &server_addr, sizeof(server_addr));
	printf("[Клиент] Серверу отправленна строка: %s\n", buff);

	char server_ip[INET_ADDRSTRLEN]; //объявим строку для адреса сервера
	inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, sizeof(server_ip)); //запишем в массив адресм сервера
	int server_port = ntohs(server_addr.sin_port); //запишем порт сервера + преобразовав его в lit-end

	printf("[Клиент] У сервера IP: %s Порт: %d\n", server_ip, server_port);

	socklen_t addr_len = sizeof(server_addr);
	ssize_t nread = recvfrom(sockfd, (char *)buff, STRING_SIZE, 0, 
			(struct sockaddr *)&server_addr, &addr_len);
	printf("[Клиент] Было записано байт: %ld\n", nread);
	printf("[Клиент] От сервера получили строку: %s\n", buff);

	close(sockfd);
	exit(EXIT_SUCCESS);
}