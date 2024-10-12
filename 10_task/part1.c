#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//getpid
#include <errno.h>

#include <sys/types.h> //завершение процесса
#include <sys/wait.h> //завершение процесса

int main(void)
{
	pid_t pid_proc;
	int ret_code;
	switch (pid_proc = fork())
	{
	case -1:
		printf("Ошибка, дочерний процесс не создан\n");
		exit(EXIT_SUCCESS);
		break;
	case 0:
		printf("[Дочерний процесс] PID = %d\n", getpid());
		printf("[Дочерний процесс] Введите код возврата \n");
		scanf("%d", &ret_code);
		exit(ret_code);
		break;
	default:
		printf("[Родительский процесс] PID = %d\n", getpid());
		wait(&ret_code);
		printf("[Родительский процесс] код.возвр. дочернего процесса = %d\n", ret_code/256);
	}
	exit(EXIT_SUCCESS);
}