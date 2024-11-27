/*
*	Данная программа выступает в роли сервера.
*	Программа использует два семафора, semid_ready и semid_done.
*	Первый используется для сигнализации, что сервер отправил
*	сообщение и его можно считать. Второй используется для
*	сигнализации, что для сервера готов ответ и его можно
*	считать. Для работы с двумя семафора используются две
*	функции sem_signal и sem_wait. В программе используется
*	стандарт System V.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SHM_SIZE 256 /*размер сегмента разделяемой памяти*/
#define SEM_KEY 7777 /*ключ для семафоров*/

void sem_wait(int semid)
{
	struct sembuf sb = {0, -1, 0}; /*определление структуры*/
	if (semop(semid, &sb, 1) == -1) /*уменьшение семафора на 1*/
	{
		perror("[Сервер] Ошибка в sem_wait semop"); /*вывод сообщения об ошибке semop*/
		exit(EXIT_FAILURE);
	}
}

void sem_signal(int semid)
{
	struct sembuf sb = {0, 1, 0}; /*определение структуры*/
	if (semop(semid, &sb, 1) == -1) /*увеличение семафора на 1*/
	{
		perror("[Сервер] Ошибка в sem_signal semop"); /*вывод сообщения об ошибке semop*/
		exit(EXIT_FAILURE);
	}
}

int main()
{
	int shm_id; /*id сегмента разделяемой памяти*/
	key_t key = ftok("shmfile", 65); /*создание ключа*/
	char *str;
	int semid_ready;
	int semid_done;

	/*создание семафора готовности*/
	semid_ready = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
	if (semid_ready == -1)
	{
		perror("[Сервер] Ошибка semget"); /*вывод сообщения об ошибке semget*/
		exit(EXIT_FAILURE);
	}
	semctl(semid_ready, 0, SETVAL, 0); /*инициализация семафора готовности*/

	/*создание семафора завершения*/
	semid_done = semget(SEM_KEY + 1, 1, 0666 | IPC_CREAT);
	if (semid_done == -1)
	{
		perror("[Сервер] Ошибка semget"); /*вывод сообщения об ошибке semget*/
		exit(EXIT_FAILURE);
	}
	semctl(semid_done, 0, SETVAL, 0); /*инициализация семафора завершения*/

	/*создание сегмента разделяемой памяти*/
	shm_id = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
	if (shm_id == -1)
	{
		perror("[Сервер] Ошибка semget"); /*вывод сообщения об ошибке shmget*/
		exit(EXIT_FAILURE);
	}

	/*подключение к сегменту разделяемой памяти*/
	str = shmat(shm_id, NULL, 0);
	if (str == (char *)(-1))
	{
		perror("[Сервер] Ошибка shmat"); /*вывод сообщения об ошибке shmat*/
		exit(EXIT_FAILURE);
	}

	/*отправка сообщения*/
	strcpy(str, "Hi!");
	printf("[Сервер] Отправленно сообщение: %s\n", str);

	sem_signal(semid_ready);

	sem_wait(semid_done);

	printf("[Сервер] Получено сообщение: %s\n", str);

	/*высвобождение ресурсов*/
	shmdt(str);
	shmctl(shm_id, IPC_RMID, NULL);
	semctl(semid_ready, 0, IPC_RMID);
	semctl(semid_done, 0, IPC_RMID);

	exit(EXIT_SUCCESS);
}
