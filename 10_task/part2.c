/*
*	Программа порождает процесс 1 и процесс 2
*	и ждёт их завершения. Процесс 1 порождает
*	процесс 3 и процесс 4 и ждёт их завершения.
*	Процесс 2 порождает процесс 5 и ждёт его
*	завершения. Каждый процесс выводит на экран
*	свой PID и PPID.
*	               
*	   (main)      
*	    /  \       
*	  (p1)  (p2)   
*	  /  \    \    
*	(p3) (p4) (p5) 
*	               
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_process_1()
{
	pid_t pid1 = fork();
	if (pid1 == 0)
	{
		//вывод PID и PPID для процесса 1
		printf("[Процесс 1]: PID = %d, PPID = %d\n", getpid(), getppid());

		pid_t pid3 = fork();
		if (pid3 == 0)
		{
			//вывод PID и PPID для процесса 3
			printf("[Процесс 3]: PID = %d, PPID = %d\n", getpid(), getppid());
			exit(0);
		}

		pid_t pid4 = fork();
		if (pid4 == 0)
		{
			//вывод PID и PPID для процесса 4
			printf("[Процесс 4]: PID = %d, PPID = %d\n", getpid(), getppid());
			exit(0);
		}

		//ожидение завершения процесса 3 и процесса 4
		wait(NULL);
		wait(NULL);
		exit(0);
	}
}

void create_process_2()
{
	pid_t pid2 = fork();
	if (pid2 == 0)
	{
		//вывод PID и PPID для процесса 2
		printf("[Процесс 2]: PID = %d, PPID = %d\n", getpid(), getppid());

		// Порождение процесса5
		pid_t pid5 = fork();
		if (pid5 == 0)
		{
			//вывод PID и PPID для процесса 5
			printf("[Процесс 5]: PID = %d, PPID = %d\n", getpid(), getppid());
			exit(0);
		}

		//ожидание завершения процесса 5
		wait(NULL);
		exit(0);
	}
}

int main()
{
	//создание процессов
	create_process_1();
	create_process_2();

	//ожидание завершения процесса1 и процесса2
	wait(NULL);
	wait(NULL);

	exit(EXIT_SUCCESS);
}