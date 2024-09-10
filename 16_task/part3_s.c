#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "part3_lib.h" //

int main ()
{
	int server = socket(AF_INET, SOCK_STREAM, 0); //создание сокета
	struct sockaddr_in adr = {0}; //создание endpoint 
	adr.sin_family = AF_INET; //тип адреса сокета
	adr.sin_port = htons(7766); //задаём порт преобразовав его в big-endian
	Bind(server, (struct sockaddr *) &adr, sizeof adr); 
	Listen(server, 5); //ожидание подключения от клиента
	socklen_t addrlen = sizeof (adr);
	int fd = Accept(server, (struct sockaddr *) &adr, &addrlen); //записываем новый файловый дес.
	ssize_t nread; //будет хранить количество считанных байт
	char buff[256];
	nread = read(fd, buff, 256); //чтение инфыормации от клиент в buff
	if (nread == -1)
	{
		perror("[Сервер] Ошибка read\n");
		exit(EXIT_FAILURE);
	}
	if (nread == 0)
	{
		printf("[Сервер] Конец файла\n");
		exit(EXIT_SUCCESS);
	}
	
	char client_ip[INET_ADDRSTRLEN]; //объявим массив для хранения адреса клиента
	inet_ntop(AF_INET, &adr.sin_addr, client_ip, sizeof(client_ip)); //запишем в массив адресм клиента
	int client_port = ntohs(adr.sin_port); //запишем порт клиента + преобразовав его в lit-end

	printf("[Сервер] У клиента IP: %s Порт: %d\n", client_ip, client_port);
	printf("[Сервер] Было записано байт: %d\n", nread);
	printf("[Сервер] От клиента получили строку: %s\n", buff);
	//write(STDOUT_FILENO, buff, nread); //печать полученной строки на экран
	buff[nread] = '+'; // добавление в троку '+'
	write(fd, buff, nread+1); //отправляем новую строку клиенту

	sleep(1); //задержка на 1 секунду
	close(fd);
	close(server);
	exit(EXIT_SUCCESS);
}