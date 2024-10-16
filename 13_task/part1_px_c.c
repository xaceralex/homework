/*
*	Данная программа выступает в роли клиента. Клиент
*	подключается к очередям сообщений, к первой для
*	отправки, ко второй для получения сообщений. Далее
*	получает сообщение и отправляет ответ с текстом
*	«Hello!». В процессе работы, полученное и
*	отправленное, сообщения выводится на экран.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME "/server_queue"
#define CLIENT_QUEUE_NAME "/client_queue"
#define MSG_SIZE 256
#define MSG_TEXT "Hello!"

int main()
{
	mqd_t server_mq, client_mq;
	char msg[MSG_SIZE];

	//открытие очереди сообщений для сервера
	server_mq = mq_open(SERVER_QUEUE_NAME, O_RDONLY);
	if (server_mq == (mqd_t)-1)
	{
		perror("[Клиент] Ошибка mq_open:");
		exit(EXIT_FAILURE);
	}

	//открытие очереди сообщений для клиента
	client_mq = mq_open(CLIENT_QUEUE_NAME, O_WRONLY);
	if (client_mq == (mqd_t)-1)
	{
		perror("[Клиент] Ошибка mq_open:");
		exit(EXIT_FAILURE);
	}

	//получение сообщения из очереди
	if (mq_receive(server_mq, msg, MSG_SIZE, NULL) == -1)
	{
		perror("[Клиент] Ошибка mq_receive:");
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Получено сообщение: %s\n", msg);

	//отправка сообщения в очередь
	strcpy(msg, MSG_TEXT);
	if (mq_send(client_mq, msg, sizeof(msg), 0) == -1)
	{
		perror("[Клиент] Ошибка mq_send:");
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Отправленно сообщение: %s\n", msg);

	//закрытие очереди
	mq_close(server_mq);
	mq_close(client_mq);

	exit(EXIT_SUCCESS);
}