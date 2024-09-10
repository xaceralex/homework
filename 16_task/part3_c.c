#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "part3_lib.h"

int main()
{
	int fd = Socket(AF_INET, SOCK_STREAM, 0); //объявляем сокет
	struct sockaddr_in adr = {0}; //объявляем endpoint, обнуляем его
	adr.sin_family = AF_INET; //указываем для endpoint протокол IPv4
	adr.sin_port = htons(7766); //указываем для endpoint порт 7766
	Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr); //преобразует IP в бинарный вид
	Connect(fd, (struct sockaddr *) &adr, sizeof adr);
	write(fd, "Test text", 9); //отправляем строку на сервер
	char buff[256];
	ssize_t nread;
	nread = read(fd, buff, 256); //считываем сообщение от сервера
	if (nread == -1) 
	{
		perror("[Клиент] Ошибка read");
		exit(EXIT_FAILURE);
	}
	if (nread == 0)
	{
		printf("[Клиент] Конец файла\n");
	}

	char server_ip[INET_ADDRSTRLEN]; //объявим массив для хранения адреса сервера
	inet_ntop(AF_INET, &adr.sin_addr,  server_ip, sizeof(server_ip)); //запишем в массив адресм сервера
	int server_port = ntohs(adr.sin_port); //запишем порт сервера + преобразовав его в lit-end
	printf("[Клиент] У сервера IP: %s Порт: %d\n", server_ip, server_port);
	printf("[Клиент] От сервера получили строку: %s\n", buff);
	close(fd);
	exit(EXIT_SUCCESS);
}