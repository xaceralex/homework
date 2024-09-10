#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7766
#define STRING_SIZE 256

int main()
{
	int sockfd; //создание сокета
	char buff[STRING_SIZE];
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //создаём сокет
	if (sockfd == -1)
	{
		perror("[Клиент] Ошибка функции socket");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr)); //настраиваем адрес сервера
	server_addr.sin_family = AF_INET; //указываем протокол IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY; //получение запросов со всех интерфесов
	server_addr.sin_port = htons(PORT); //задаём порт

	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)// Привязываем сокет к адресу
	{
		perror("Ошибка привязки сокета");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	ssize_t nread = recvfrom(sockfd, (char *)buff, STRING_SIZE, 0,(struct sockaddr *) &client_addr, &client_addr_len);
	buff[nread] = '\0'; //закрываем строку
	//printf("[Сервер] Клиент отправил строку: %s\n", buff);

	char client_ip[INET_ADDRSTRLEN]; //объявим строку для адреса клиента
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)); //запишем в массив адресм клиента
	int client_port = ntohs(client_addr.sin_port); //запишем порт клиента + преобразовав его в lit-end

	printf("[Сервер] У клиента IP: %s Порт: %d\n", client_ip, client_port);
	printf("[Сервер] Было записано байт: %ld\n", nread);
	printf("[Сервер] От клиента получили строку: %s\n", buff);

	buff[nread] = '+'; // добавление в троку '+'
	printf("[Сервер] Клиенту отправили строку: %s\n", buff);
	
	sendto(sockfd, buff, strlen(buff), 0, 
			(const struct sockaddr *) &client_addr, client_addr_len);
	
	close(sockfd);
	exit(EXIT_SUCCESS);
}