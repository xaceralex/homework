/*
*	Данная программа выступает в роли сервера. Сервер
*	создаёт именованный канал и открывает его только 
*	на запись, записывает в него строку "Hi!" и 
*	завершается.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FIFO_NAME "my_fifo"

int main()
{
	int fd;

	//создание именованного канала (FIFO)
	if (mkfifo(FIFO_NAME, 0666) == -1)
	{
		perror("Ошибка mkfifo");
		exit(EXIT_FAILURE);
	}

	
	fd = open(FIFO_NAME, O_WRONLY); //открытие канала только на запись
	if (fd == -1)
	{
		perror("[Сервер] Ошибка open");
		exit(EXIT_FAILURE);
	}

	const char *message = "Hi!";
	write(fd, message, strlen(message) + 1); //запись строки в канал записи

	close(fd);

	exit(EXIT_SUCCESS);
}