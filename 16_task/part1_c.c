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
	int ret; // для проверок
	int server_socket;
	char buffer[STRING_SIZE];
	struct sockaddr_un name;

	memset(buffer,0,STRING_SIZE);

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0); //создание сокета
	if (server_socket == -1) //проверка для socket
	{
		printf("[Клиент] Ошибка создания сокета\n");
		exit(EXIT_SUCCESS);
	}

	memset(&name, 0, sizeof(STRING_SIZE));

	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

	ret = connect(server_socket, (const struct sockaddr *) &name,
		sizeof(name));
	if (ret == -1) //проверка для connect
	{
		printf("[Клиент] Ошибка connect\n");
		exit(EXIT_SUCCESS);
	}

	strcpy(buffer, "Text_cl_");

	ret = write(server_socket, buffer, sizeof(buffer));
	if (ret == -1) //проверка для write
	{
		printf("[Клиент] Ошибка write\n");
		exit(EXIT_SUCCESS);
	}

	ret = read(server_socket, buffer, STRING_SIZE);
	if (ret == -1) //проверка для read
	{
		printf("[Клиент] Ошибка read\n");
		exit(EXIT_SUCCESS);
	}
	printf("[Клиент] Получена строка =%s\n",buffer);

	close(server_socket);
	exit(EXIT_SUCCESS);
}