/*
*	Данная программа выступает в роли условного
*	сервера, который принимает сигнал от второй
*	программы(клиента). Чтобы можно было отправить
*	сигнал программа выводит свой PID. При
*	получении сигнала выводится соответствующее
*	сообщение.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigusr1()
{
	printf("[Сервер] Получен сигнал SIGUSR1\n");
}

int main()
{
	struct sigaction sa;
	sa.sa_handler = handle_sigusr1;
	sa.sa_flags = 0;

	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("[Сервер] Ошибка sigaction");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] PID: %d\n", getpid());
	printf("[Сервер] Ожидание сигнала SIGUSR1\n");

	while (1)
	{
		pause(); //ожидание сигнала
	}

	exit(EXIT_SUCCESS);
}