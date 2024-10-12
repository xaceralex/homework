/*
*	Эта программа выступает в роли "слушающего"
*	сервера. Сервер заранее создаёт несколько(5)
*	потоков. Далее ожидает подключения клиента.
*	После подключения клиента сервер отправляет
*	endpoint клиента в очередь сообщений и любой
*	свободный поток считывает этот endpoint и
*	начинает обслуживать клиента. Поток получает
*	сообщение от клиента, преобразовывает его и
*	отправляет обратно.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PORT 7777
#define SIZE 256
#define MAX_THREADS 5
#define MSG_TYPE 1

//структура которую будем передавать в очередь сообщений
struct msgbuff {
	long mtype;
	int client_socket;
};

void *handle_client(void *arg)
{
	struct msgbuff msg;
	char buffer[SIZE];
	int read_bytes = 0;

	pthread_t thread_id = pthread_self();// получим id потока для отличия
	while (1)
	{
		//получение сообщения из очереди
		msgrcv(*(int *)arg, &msg, sizeof(msg.client_socket), MSG_TYPE, 0);

		//сервер будет получать и отправлять сообщения в течении ~20 сек.
		for (int i = 0; i < 20; i++)
		{
			read_bytes = recv(msg.client_socket, buffer, sizeof(buffer) - 1, 0); //получение сообщения от клиента
			if (read_bytes > 0)
			{
				buffer[read_bytes] = '\0'; //закрываем строку

				printf("[Сервер new(%lu)] Получено от клиента: %s\n", (unsigned long)thread_id, buffer);
				printf("[Сервер new(%lu)] Получено байт: %d\n",(unsigned long)thread_id, read_bytes);

				//преобразование сообщения клиента
				buffer[read_bytes] = '+'; //изменяем строку
				buffer[read_bytes+1] = '\0'; //закрываем строку
				read_bytes++;

				send(msg.client_socket, buffer, read_bytes, 0); //отправляем преобразованное сообщение обратно клиенту
				read_bytes = 0;
			}
			sleep(1);
		}
		close(msg.client_socket);
	}
	return NULL;
}

int main()
{
	int server_socket, client_socket;
	int msgid;
	struct sockaddr_in server_addr, client_addr;
	struct msgbuff msg;
	socklen_t client_addr_len = sizeof(client_addr);

	//создание очереди сообщений
	msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
	if (msgid < 0)
	{
		perror("[Сервер] Ошибка msgget");
		exit(EXIT_FAILURE);
	}

	//создание потоков(обслуживающих серверов)
	pthread_t thread;
	for (int i = 0; i < MAX_THREADS; i++)
	{
		if(pthread_create(&thread, NULL, handle_client, (void *)&msgid) != 0)
		{
			perror("[Сервер] Ошибка pthread_create");
			exit(EXIT_FAILURE);
		}
		pthread_detach(thread);
	}

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("[Сервер] Ошибка");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[Сервер] Ошибка bind");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	listen(server_socket, 5);
	printf("[Сервер] Идёт прослушивание на порту: %d\n", PORT);

	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_socket < 0)
		{
			perror("[Сервер] Ошибка accept");
			continue;
		}

		printf("[Сервер] Клиент подключился к серверу: %s\n", inet_ntoa(client_addr.sin_addr));

		//отправка сокета клиента в очередь сообщений
		msg.mtype = MSG_TYPE;
		msg.client_socket = client_socket;
		msgsnd(msgid, &msg, sizeof(msg.client_socket), 0);
	}
	close(server_socket);
	exit(EXIT_SUCCESS);
}