#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/un.h>
#include <unistd.h>

#include <errno.h> //обработка ошибок
#define STRING_SIZE 10
#define SOCKET_NAME_SR "/tmp/socket_server"

int main(void)
{
	int server_socket;
	struct sockaddr_un name_client, name_server;
	memset(&name_server,0,sizeof(struct sockaddr_un));
	int ret;
	char buffer [STRING_SIZE];

	memset(buffer,0,STRING_SIZE); //очистка буфера
	remove(SOCKET_NAME_SR); //удаление сокета

	server_socket = socket(AF_UNIX, SOCK_DGRAM, 0);

	if (server_socket == -1) //проверка для socket(сервер)
	{
		printf("[Сервер] Ошибка создания сокета для сервера\n");
		exit(EXIT_SUCCESS);
	}

	name_server.sun_family = AF_UNIX; // заполнение структуры n-поинт
	strncpy(name_server.sun_path, SOCKET_NAME_SR, strlen(SOCKET_NAME_SR)+1);

	ret = bind(server_socket, (struct sockaddr *) &name_server, sizeof(struct sockaddr_un));

	if (ret == -1) //проверка для bind
	{
		printf("[Сервер] Ошибка bind\n");
		printf("[Сервер] Ошибка errno =%s\n", strerror(errno));
		exit(EXIT_SUCCESS);
	}

	int size_struct = sizeof(struct sockaddr_un);
	ret = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &name_client, &size_struct);

	if (ret == -1) //проверка для bind
		{
			printf("[Сервер] Ошибка recvfrom\n");
			exit(EXIT_SUCCESS);
		}

	printf("[Сервер] Получена строка =%s\n",buffer);
	printf("[Сервер] sun_path клиента =%s\n", name_client.sun_path);

	strcpy(buffer, "Text_cl_+");
	printf("[Сервер] Новая строка =%s\n", buffer);

	ret = sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &name_client, sizeof(struct sockaddr_un));

	close(server_socket);
	exit(EXIT_SUCCESS);
}