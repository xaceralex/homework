/*
*	Программа использует два семафора semid_ready
*	и semid_done. Первый используется для сигнализации,
*	что сервер отправил сообщение и его можно считать.
*	Второй используется для сигнализации, что для сервера
*	готов ответ и его можно считать. Для работы с двумя
*	семафора используются две функции sem_post и sem_wait.
*	В программе используется стандарт POSIX.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>

#define SHM_NAME "/my_shm" /*имя для разделяемой памяти*/
#define SEM_READY "/sem_ready" /*имя для семафора готовности*/
#define SEM_DONE "/sem_done" /*имя для семафора завершения*/
#define SHM_SIZE 256 /*размер сегмента разделяемой памяти*/

int main()
{
	int shm_fd; /*дескриптор сегмента разделяемой памяти*/
	char *str;

	/*cоздание сегмента разделяемой памяти*/
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
	ftruncate(shm_fd, SHM_SIZE); /*определение размера для сегмента*/
	str = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	/*создание семафоров*/
	sem_t *sem_ready = sem_open(SEM_READY, O_CREAT, 0666, 0);
	sem_t *sem_done = sem_open(SEM_DONE, O_CREAT, 0666, 0);
	
	/*отправка сообщения*/
	strcpy(str, "Hi!");
	printf("[Сервер] Отправлено сообщение: %s\n", str);

	sem_post(sem_ready);

	sem_wait(sem_done);

	printf("[Сервер] Получено сообщение: %s\n", str);

	/*освобождение ресурсов*/
	munmap(str, SHM_SIZE);
	shm_unlink(SHM_NAME); //удаляем сегмент разделяемой памяти
	sem_close(sem_ready);
	sem_close(sem_done);
	sem_unlink(SEM_READY);
	sem_unlink(SEM_DONE);

	exit(EXIT_SUCCESS);
}
