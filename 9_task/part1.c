#include <stdio.h>
#include <stdlib.h>
//для работы с файлами
#include <unistd.h>

//определения аргументов open
#include <fcntl.h>
//обработка ошибок
#include <errno.h>
#include <string.h>

extern int errno;

int main(void) {
	int dis_num;
	char text[16] = "String from file";

	if (dis_num = open("output.txt", O_CREAT,0775) == -1)
	{
		printf("Ошибка создания файла\n");
	} else
		printf("Файл успешно создан\n");

	dis_num = open("output.txt", O_RDWR);
	write(dis_num, text, sizeof(text));

	dis_num = open("output.txt", O_RDWR);

	char text_rev[16] = "";

	for (int i = -1, j = 0; j < sizeof(text); --i, j++)
	{
		if (lseek(dis_num, i, SEEK_END) == -1)
		{
			printf("Номер ошибки: %d\n",errno);
			printf("Ошибка открытия файла: %s\n", strerror(errno));
		}
		read (dis_num, text_rev+j, 1);
	}

	printf("Строка из файла:%s\n",text_rev);

	close(dis_num);
	exit(EXIT_SUCCESS);
}