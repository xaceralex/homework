#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/inotify.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>

#define CLIENT_NAME_LEN 50		/*макс. длина имени клиента*/
#define CLIENT_COUNT_MAX 10		/*макс. кол-во клиентов*/
#define MESSAGES_COUNT_MAX 80	/*макс. кол-во сообщений*/
#define MESSAGES_LEN_MAX 100	/*макс. длина одного сообщения*/

typedef struct {
	int version;											/*версия чата*/
	int messages_count;										/*количество сообщений*/
	int clients_count;										/*количество клиентов*/
	char clients[CLIENT_COUNT_MAX][CLIENT_NAME_LEN];		/*списко клиентов*/
	char messages[MESSAGES_COUNT_MAX][MESSAGES_LEN_MAX];	/*список сообщений*/
	char client_in[CLIENT_NAME_LEN];						/*клиент который подключается к чату*/
	char client_out[CLIENT_NAME_LEN];						/*клиент который выходит из чата*/
	pthread_mutex_t clients_mutex;							/*мьютекс для списка клиентов*/
	pthread_mutex_t messages_mutex;							/*мьютекс для списка сообщений*/
} chat_data;

chat_data *chat;

int shm_id;

void* shm_clients(void* arg)
{
	char message_total[MESSAGES_LEN_MAX];
	while (1)
	{
		if (chat->client_in[0] != '*')
		{
			pthread_mutex_lock(&chat->clients_mutex);
			strcpy(chat->clients[chat->clients_count], chat->client_in);

			pthread_mutex_lock(&chat->messages_mutex);
			snprintf(message_total, sizeof(message_total), "%s присоединился(-ась) к чату", chat->client_in);
			strcpy(chat->messages[chat->messages_count], message_total);
			chat->messages_count++;
			pthread_mutex_unlock(&chat->messages_mutex);

			memset(chat->client_in, 0, CLIENT_NAME_LEN);
			chat->client_in[0] = '*';
			chat->clients_count++;
			chat->version++;
			pthread_mutex_unlock(&chat->clients_mutex);
		}

		if (chat->client_out[0] != '*')
		{
			pthread_mutex_lock(&chat->clients_mutex);

			for (int i = 0; i < chat->clients_count; i++)
			{
				if (strcmp(chat->clients[i], chat->client_out) == 0)
				{
					for (int j = i; j < chat->clients_count - 1; j++)
					{
						strcpy(chat->clients[j], chat->clients[j + 1]);
					}
						chat->clients_count--;
						memset(chat->clients[chat->clients_count], 0, CLIENT_NAME_LEN);
						break;
				}
			}
			pthread_mutex_lock(&chat->messages_mutex);
			snprintf(message_total, sizeof(message_total), "%s покинул(-а) чат", chat->client_out);
			strcpy(chat->messages[chat->messages_count], message_total);
			chat->messages_count++;
			memset(chat->client_out, 0, CLIENT_NAME_LEN);
			chat->client_out[0] = '*';
			chat->version++;
			pthread_mutex_unlock(&chat->messages_mutex);
			pthread_mutex_unlock(&chat->clients_mutex);
		}
		sleep(1);
	}
}

int main()
{
	pthread_t thread_shm_clients;
	setlocale(LC_ALL, "ru_RU.UTF-8"); /*подключение русской локали*/

	int shm_id = shmget(777, sizeof(chat_data), IPC_CREAT | 0666); /*создание сегмента разделяемой памяти*/
	if (shm_id == -1)
	{
		perror("[Сервер] Ошибка shmget");
		return EXIT_FAILURE;
	}

	/*подключение разделяемой памяти к адр. простр. проц.*/
	chat = (chat_data*)shmat(shm_id, NULL, 0);
	if (chat == (chat_data *)(-1))
	{
		perror("[Сервер] Ошибка shmat");
		return EXIT_FAILURE;
	}

	/*инициализация мьютекс*/
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&chat->clients_mutex, &attr);
	pthread_mutex_init(&chat->messages_mutex, &attr);

	memset(chat, 0, sizeof(chat_data)); /*очищаем структуру*/

	chat->version = 0; /*начальная версия чата*/
	chat->messages_count = 0; /*начальное кол-во сообщений*/
	chat->clients_count = 0; /*начальное кол-во клиентов*/

	chat->client_in[0] = '*'; /*помещаем в поле символ-флаг*/
	chat->client_out[0] = '*'; /*помещаем в поле символ-флаг*/

	/*запишем в структуру несколько сообщений для примера*/
	strcpy(chat->messages[0], "Сергей: Всем привет");
	strcpy(chat->messages[1], "Алексей: Тебе тоже привет");
	strcpy(chat->messages[2], "Максим: Где Вова?");

	/*запишем в структуру несколько клиентов для примера*/
	strcpy(chat->clients[0], "Сергей");
	strcpy(chat->clients[1], "Алексей");
	strcpy(chat->clients[2], "Максим");
	strcpy(chat->clients[3], "Владимир");
	strcpy(chat->clients[4], "Александр");

	/*обновление информации о состоянии чата*/
	chat->version = 6;
	chat->messages_count = 3;
	chat->clients_count = 5;

	/*создание потока для чтения чата в фоново режиме*/
	if (pthread_create(&thread_shm_clients, NULL, shm_clients, NULL) != 0)
	{
		perror("[Сервер] Ошибка pthread_create");
		exit(EXIT_FAILURE);
	}

	getchar(); /*ожидание ввода до завершения программы*/

	memset(chat, 0, sizeof(chat_data)); /*очистка структуры*/

	if (shmdt(chat) == -1)
	{
		perror("[Сервер] Ошибка shmdt");
		return EXIT_FAILURE;
	}

	if (shmctl(shm_id, IPC_RMID, NULL) == -1)
	{
		perror("[Сервер] Ошибка shmctl");
		return EXIT_FAILURE;
	}
	exit(EXIT_SUCCESS);
}
