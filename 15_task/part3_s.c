/*
*	Данная программа блокирует сигнал(SIGUSR1),
*	и переходит в бесконечный цикл ожидания
*	сигнала, при получении сигнала на экран
*	выводится сообщение о получении сигнала и
*	осуществляется переход на следующую
*	итерацию цикла.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int main()
{
	sigset_t set;
	int sig;

	printf("[Сервер] PID: %d\n", getpid());

	//блокировка сигнала SIGUSR1
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	printf("[Сервер] Вход в бесконечный цикл: %d\n", getpid());

	//цикл с ожиданием сигнала
	while (1)
	{
		//ожидание сигнала SIGUSR1
		sigwait(&set, &sig);

		//обработка полученного сигнала
		if (sig == SIGUSR1)
		{
			printf("[Сервер] Получен сигнал SIGUSR1!\n");
		}
	}

	exit(EXIT_SUCCESS);
}