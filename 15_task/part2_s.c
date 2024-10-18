/*
*	В данной программе реализована блокировка сигнала
*	SIGINT(Ctrl + C). После блокировки сигнала
*	программа уходит в бесконечный цикл, в котором
*	можно попробовать отправить сигнал SIGINT.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	sigset_t new_mask, old_mask;

	//создание новой макси, добавление SIGINT в маску
	sigemptyset(&new_mask);
	sigaddset(&new_mask, SIGINT);

	printf("[Сервер] PID: %d\n", getpid());

	//блокирование SIGINT
	if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) == -1)
	{
		perror("[Сервер] Ошибка sigprocmask");
		exit(EXIT_FAILURE);
	}

	printf("[Сервер] Сигнал SIGINT заблокирован. Вход бесконечный цикл\n");

	//бесконечный цикл
	while (1)
	{
		sleep(1);
	}

	//отключение блокировки (строка недостижима)
	sigprocmask(SIG_SETMASK, &old_mask, NULL);

	exit(EXIT_SUCCESS);
}