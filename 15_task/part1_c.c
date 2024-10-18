/*
*	Данная программа выступает в роли условного
*	клиента, который отправляет сигнал(SIGUSR1)
*	первой программе(серверу). Для отправки
*	сначала нужно ввести PID первой программы(сервера).
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	pid_t pid;

	printf("[Клиент] Введите PID для отправки сигнала: ");
	scanf("%d", &pid);

	if (kill(pid, SIGUSR1) == -1)
	{
		perror("[Клиент] Ошибка kill");
		exit(EXIT_FAILURE);
	}

	printf("[Клиент] Отправлен сигнал SIGUSR1 процессу с PID: %d\n", pid);
	exit(EXIT_SUCCESS);
}