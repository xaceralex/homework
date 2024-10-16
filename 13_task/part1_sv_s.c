/*
*	Данная программа выступает в роли сервера. Сервер
*	создаёт очередь сообщений, отправляет в неё сообщение
*	с текстом «Hi!». Далее ожидает ответа из очереди.
*	После получения ответа сервер удаляет очередь. В
*	процессе работы, отправленное и полученное, сообщения
*	выводится на экран.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 256 //размер сообщения
#define MSG_TEXT "Hi!" //текст сообщения

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
	message.type = 1; //тип сообщения

	//генерация уникального ключа для очереди сообщений
	if ((key = ftok("part1_sv_s", 77)) == -1)
	{
		perror("[Сервер] Ошибка ftok");
		exit(EXIT_FAILURE);
	}

	//создание очереди сообщений
	if ((msg_id = msgget(key, 0666 | IPC_CREAT)) == -1)
	{
		perror("[Сервер] Ошибка msgget");
		exit(EXIT_FAILURE);
	}

	strcpy(message.text, MSG_TEXT);

	//отправка сообщения в очередь
	if (msgsnd(msg_id, &message, sizeof(message.text), 0) == -1)
	{
		perror("[Сервер] Ошибка msgsnd");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] В очередь сообщений отправленно: %s\n", message.text);

	//ожидание ответа из очереди
	msgrcv(msg_id, &message, sizeof(message.text), 2, 0);

	printf("[Сервер] Из очереди сообщений получено: %s\n", message.text);

	//удаление очереди
	if (msgctl(msg_id, IPC_RMID, NULL) == -1)
	{
		perror("[Сервер] Ошибка msgctl");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}