/*
*	программа сервер для обмена пакетами
*	с клиентом используя протоколы tcp и 
*	AF_UNIX
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/un.h>
#include <unistd.h>

#define STRING_SIZE 10
#define SOCKET_NAME "/tmp/socket"

int main(void)
{
	int ret;
	int server_socket, client_socket;
	int r, w;
	char buffer [STRING_SIZE];
	struct sockaddr_un name;

	memset(buffer,0,STRING_SIZE); // очистка буффера
	unlink(SOCKET_NAME); // удаление сокета

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_socket == -1) //проверка для socket
	{
		printf("[Сервер] Ошибка создания сокета\n");
		exit(EXIT_FAILURE);
	}

	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

	ret = bind(server_socket, (const struct sockaddr *) &name, 
		sizeof(struct sockaddr_un));
	if (ret == -1) //проверка для bind
		{
			printf("[Сервер] Ошибка bind\n");
			exit(EXIT_FAILURE);
		}

	ret = listen(server_socket, 5);
	if (ret == -1) //проверка для listen
	{
		printf("[Сервер] Ошибка listen\n");
		exit(EXIT_FAILURE);
	}

	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1) //проверка для accept
	{
		printf("[Сервер] Ошибка accept\n");
		exit(EXIT_FAILURE);
	}

	r = read(client_socket, buffer, sizeof(buffer));
	if (r == -1) //проверка для read
	{
		printf("[Сервер] Ошибка read\n");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Получена строка =%s\n",buffer);

	buffer[STRING_SIZE-2] = '+'; // изменение полученной строки

	w = write(client_socket, buffer, sizeof(buffer));
	if (w == -1) //проверка для write
	{
		printf("[Сервер] Ошибка write\n");
		exit(EXIT_FAILURE);
	}

	close(client_socket);
	close(server_socket);
	exit(EXIT_SUCCESS);
}