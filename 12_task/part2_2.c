/*
*	Данная программа выступает в роли клиента. Клиент 
*	открывает созданный сервером канал только на 
*	чтение. Считывает строку из канала, выводит её на
*	экран. Удаляет канал.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "my_fifo"

int main()
{
	int fd = open(FIFO_NAME, O_RDONLY); // открытие канала только на чтение
	char buffer[100]; //строка для записи
	if (fd == -1)
	{
		perror("[Клиент] Ошибка open");
		exit(EXIT_FAILURE);
	}

	read(fd, buffer, sizeof(buffer)); //чтение строки из канала

	close(fd); //закрытие канала

	printf("Client received: %s\n", buffer); //вывод полученной строки

	unlink(FIFO_NAME); //удаление канала

	exit(EXIT_SUCCESS);
}