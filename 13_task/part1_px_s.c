/*
*	Данная программа выступает в роли сервера. Сервер
*	создаёт две очереди сообщений, одну для отправки,
*	другую для получения сообщений. Отправляет сообщение
*	с текстом «Hi!». Далее ожидает ответа из второй
*	очереди. После получения ответа сервер удаляет очереди.
*	В процессе работы, отправленное и полученное, сообщения
*	выводится на экран.
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
#define MSG_TEXT "Hi!"

int main()
{
	mqd_t server_mq, client_mq;
	struct mq_attr attr;
	char msg[MSG_SIZE];

	//настройка атрибутов очереди
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MSG_SIZE;
	attr.mq_curmsgs = 0;

	//создание очереди сообщений для сервера
	server_mq = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
	if (server_mq == (mqd_t)-1)
	{
		perror("[Сервер] Ошибка mq_open(SERVER_QUEUE):");
		exit(EXIT_FAILURE);
	}

	//создание очереди сообщений для клиента
	client_mq = mq_open(CLIENT_QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
	if (client_mq == (mqd_t)-1)
	{
		perror("[Сервер] Ошибка mq_open(CLIENT_QUEUE):");
		exit(EXIT_FAILURE);
	}

	//отправка сообщения в очередь
	strcpy(msg, MSG_TEXT);
	if (mq_send(server_mq, msg, sizeof(msg), 0) == -1)
	{
		perror("[Сервер] Ошибка mq_send");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Отправленно сообщение: %s\n", msg);
	
	//ожидание ответа из очереди
	if (mq_receive(client_mq, msg, MSG_SIZE, NULL) == -1)
	{
		perror("[Сервер] Ошибка mq_receive");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Получено сообщение: %s\n", msg);

	//закрытие и удаление очередей
	mq_close(server_mq);
	mq_close(client_mq);
	mq_unlink(SERVER_QUEUE_NAME);
	mq_unlink(CLIENT_QUEUE_NAME);

	exit(EXIT_SUCCESS);
}