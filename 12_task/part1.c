/*
*	Программа создаёт неименованный канал и порождает процесс.
*	Родительский процесс закрывает дискриптор для чтения и 
*	записывает в канал строку "Hi!", и ждёт завершения дочернего
*	процесса. Дочерний процесс закрывает дискриптор для записи,
*	считывает строку из канала и выводт её на экран.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int fd[2]; //дискрипторы, fd[0] для чтения, fd[1] для записи
	pid_t pid;

	//создание неименованного канала
	if (pipe(fd) == -1)
	{
		perror("Ошибка pipe");
		exit(EXIT_FAILURE);
	}

	// Порождаем дочерний процесс
	pid = fork();
	if (pid == -1)
	{
		perror("Ошибка fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0) //для родительского процесса
	{
		close(fd[0]); //закрытие дискриптора для чтения
		const char *message = "Hi!";

		write(fd[1], message, strlen(message) + 1); //запись строки в канал записи

		close(fd[1]); //закрытие дискриптора для записи
		wait(NULL); //ожидание завершения дочернего процесса
	} else //для дочернего процесса
	{
		close(fd[1]); //закрытие дискриптора для записи

		char buffer[100];

		read(fd[0], buffer, sizeof(buffer)); //чтение строки из канала

		close(fd[0]); //закрытие дискриптора для чтения

		printf("Дочерний процесс получил строку: %s\n", buffer); //вывод полученной строки
	}

	exit(EXIT_SUCCESS);
}