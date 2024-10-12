/*
*	программа клиент для обмена пакетами
*	с сервером используя протоколы udp и 
*	AF_UNIX
*/

/*	структура sockaddr_un					*/
/*	struct sockaddr_un						*/
/*	{										*/
/*		sa_family_t sun_family;		AF_UNIX */
/*		char	sun_path[108];		имя пути*/
/*	};										*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/un.h>
#include <unistd.h>
#include <errno.h> //обработка ошибок

#define STRING_SIZE 10
#define SOCKET_NAME_CL "/tmp/socket_client"
#define SOCKET_NAME_SR "/tmp/socket_server"

int main(void)
{
	int client_socket;
	struct sockaddr_un name_client, name_server;
	memset(&name_client,0,sizeof(struct sockaddr_un));
	int ret; //для проверок
	char buffer[STRING_SIZE]; //отправляемая строка

	remove(SOCKET_NAME_CL); //чистим сокет
	memset(buffer,0,STRING_SIZE); //чистим строку
	strcpy(buffer, "Text_cl_"); //определяем строку

	client_socket = socket(AF_UNIX, SOCK_DGRAM, 0); //создание сокета
	if (client_socket == -1) //проверка для socket
	{
		printf("[Клиент] Ошибка socket\n");
		exit(EXIT_FAILURE);
	}

	name_server.sun_family = AF_UNIX;
	name_client.sun_family = AF_UNIX;

	strncpy(name_server.sun_path, SOCKET_NAME_SR, sizeof(name_server.sun_path) - 1);
	strncpy(name_client.sun_path, SOCKET_NAME_CL, sizeof(name_client.sun_path) - 1);

	ret = bind(client_socket, (struct sockaddr *) &name_client, sizeof(struct sockaddr_un));

	if (ret == -1) //проверка для bind
	{
		printf("[Клиент] Ошибка bind\n");
		printf("Ошибка: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	socklen_t size_struct = sizeof(struct sockaddr_un);
	ret = sendto(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &name_server, sizeof(struct sockaddr_un));

	ret = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &name_server, &size_struct);
	printf("[Клиент] Получена строка =%s\n", buffer);
	printf("[Клиент] sun_path сервера =%s\n", name_server.sun_path);

	close(client_socket);
	exit(EXIT_SUCCESS);
}