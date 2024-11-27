/*
*	Данная программа выступает в роли клиента.
*	Программа использует два семафора, semid_ready
*	и semid_done. Первый сигнализирует, что сервер
*	отправил сообщение и его можно считать. Второй,
*	что серверу был отправлен ответ и сервер может
*	его считать. Для работы с семафорами используются
*	функции sem_wait и sem_signal. В программе
*	используется стандарт System V.
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
	struct sembuf sb = {0, -1, 0}; /*уменьшение семафора на 1*/
	if (semop(semid, &sb, 1) == -1)
	{
		perror("[Клиент] Ошибка в sem_wait semop"); /*вывод сообщения об ошибке semop*/
		exit(EXIT_FAILURE);
	}
}

void sem_signal(int semid)
{
	struct sembuf sb = {0, 1, 0}; /*увеличение семафора на 1*/
	if (semop(semid, &sb, 1) == -1)
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
	int semid_ready; /*идентификатор семафора готовности*/
	int semid_done; /*идентификатор семафора завершения*/

	/*получение доступа к семафору готовности*/
	semid_ready = semget(SEM_KEY, 1, 0666);
	if (semid_ready == -1)
	{
		perror("[Клиент] Ошибка semget"); /*вывод сообщения об ошибке semget*/
		exit(EXIT_FAILURE);
	}

	/*получение доступа к семафору завершения*/
	semid_done = semget(SEM_KEY + 1, 1, 0666);
	if (semid_done == -1)
	{
		perror("[Клиент] Ошибка semget"); /*вывод сообщения об ошибке semget*/
		exit(EXIT_FAILURE);
	}

	/*подключение к сегменту разделяемой памяти*/
	shm_id = shmget(key, SHM_SIZE, 0666);
	if (shm_id == -1)
	{
		perror("[Клиент] Ошибка shmget"); /*вывод сообщения об ошибке shmget*/
		exit(EXIT_FAILURE);
	}

	/*получение сообщения от сервера*/
	str = shmat(shm_id, NULL, 0);
	if (str == (char *)(-1))
	{
		perror("[Клиент] Ошибка shmat"); /*вывод сообщения об ошибке shmat*/
		exit(EXIT_FAILURE);
	}

	sem_wait(semid_ready); /*ожидание готовности сервера*/

	printf("[Клиент] Получено сообщение: %s\n", str); /*чтение сообщения от сервера*/

	strcpy(str, "Hello!"); /*отправка ответа серверу*/

	sem_signal(semid_done); /*сигнализируем серверу о отправке сообщения*/

	shmdt(str); /*высвобождение ресурсов*/

	exit(EXIT_SUCCESS);
}
