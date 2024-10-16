/*
*	Данная программа выступает в роли клиента. Клиент
*	подключается к очереди сообщений, получает из неё
*	сообщение. Далее отправляет ответ с текстом
*	«Hello!». В процессе работы, полученное и
*	отправленное, сообщения выводится на экран.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 256 //размер сообщения
#define MSG_TEXT "Hello!" //текст сообщения

//структура сообщения
struct msg_buffer {
	long type;
	char text[MSG_SIZE];
};

int main()
{
	key_t key;
	int msg_id;
	struct msg_buffer message; //сообщение

	//получение уникального ключа для очереди сообщений
	if ((key = ftok("part1_sv_s", 77)) == -1)
	{
		perror("[Клиент] Ошибка ftok");
		exit(EXIT_FAILURE);
	}

	//подключение к очереди сообщений
	if ((msg_id = msgget(key, 0666 | IPC_CREAT)) == -1)
	{
		perror("[Сервер] Ошибка msgget");
		exit(EXIT_FAILURE);
	}

	//ожидание сообщения из очереди
	if (msgrcv(msg_id, &message, sizeof(message.text), 1, 0) == -1)
	{
		perror("[Сервер] Ошибка msgrcv");
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Из очереди сообщений получено: %s\n", message.text);

	strcpy(message.text, MSG_TEXT);

	message.type = 2;
	//отправка сообщения в очередь
	if (msgsnd(msg_id, &message, sizeof(message.text), 0) == -1)
	{
		perror("[Сервер] Ошибка msgsnd");
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] В очередь сообщений отправленно: %s\n", message.text);

	exit(EXIT_SUCCESS);
}