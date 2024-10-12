/*
*	Программа представляет собой командный интерпретатор bash.
*	Пользователь вводит команды пока не введёт "exit".
*	Пользователь вводит команду. Программа пораждает новый
*	процесс в котором выполняет введённую команду.с опциями.
*	Далее программа ждёт завершения дочернего процесса.
*	После этого цикл повторяется.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024 //максимальный размер пользовательского ввода
#define MAX_ARG_COUNT    100 //максимальное количество аргументов пользовательского ввода

int main()
{
	char input[MAX_INPUT_SIZE];//строка для пользовательского ввода
	char *args[MAX_ARG_COUNT]; //массив для аргументов пользовательского ввода
	int arg_count; //количество аргументов

	while (1)
	{
		printf("bash> ");
		fflush(stdout); //сброс буффера

		//получение ввода пользователя
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			perror("Ошибка fgets");
			continue;
		}

		//удаление символа "\n"
		input[strcspn(input, "\n")] = 0;

		//проверка на ввод "exit"
		if (strcmp(input, "exit") == 0)
		{ 
			break;
		}

		arg_count = 0; //обнуляем количество аргументов
		char *token = strtok(input, " ");
		while (token != NULL)
		{
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL;

		//создание нового процесса
		pid_t pid = fork();

		if (pid < 0)
		{
			perror("Ошибка fork");
			continue;
		}

		if (pid == 0) //проверка для дочернего процесса
		{
			execvp(args[0], args);
			perror("Ошибка execvp");
			exit(EXIT_FAILURE);
		}else //для родительского процесса
		{
			wait(NULL);
		}
	}

	exit(EXIT_SUCCESS);
}