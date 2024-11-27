#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <locale.h>
#include <ctype.h>
#include <wchar.h>


#define CLIENT_NAME_LEN 50
#define CLIENT_COUNT_MAX 10
#define MESSAGES_COUNT_MAX 80
#define MESSAGES_LEN_MAX 100

typedef struct {
	int version;											/*версия чата*/
	int messages_count;										/*кол-во сообщений в структуре*/
	int clients_count;										/*кол-во клиентов в структуре*/
	char clients[CLIENT_COUNT_MAX][CLIENT_NAME_LEN];		/*списко клиентов*/
	char messages[MESSAGES_COUNT_MAX][MESSAGES_LEN_MAX];	/*список сообщений*/
	char client_in[CLIENT_NAME_LEN];						/*клиент который подключается к чату*/
	char client_out[CLIENT_NAME_LEN];						/*клиент который выходит из чата*/
	pthread_mutex_t clients_mutex;							/*мьютекс для списка клиентов*/
	pthread_mutex_t messages_mutex;							/*мьютекс для списка сообщений*/
} chat_data;

chat_data *chat;
int chat_version_client = -1;
char buffer[50];		/*буфер для сообщений*/
char name_curent[45];	/*имя клиента*/

WINDOW *win_chat;
WINDOW *win_list;
WINDOW *win_mes;

void* shm_read(void* arg)
{
	while (1)
	{
		if (chat_version_client != chat->version)
		{
			for (int i = 0; i < chat->messages_count; i++)
			{
				mvaddstr(i+1, 2, chat->messages[i]);
			}
			for (int i = 0; i < chat->clients_count; i++)
			{
				mvaddstr(i+1, 125, chat->clients[i]);
			}
			wclear(win_chat);
			wclear(win_list);
			box(win_chat, 0, 0);
			box(win_list, 0, 0);
			wrefresh(win_chat);
			wrefresh(win_list);

			move(36, 2);

			refresh();
		}
		chat_version_client = chat->version;
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t thread_chat_read;			/*поток для чтения чата в фоновом режиме*/
	setlocale(LC_ALL, "ru_RU.UTF-8");	/*подключение русской локали*/
	move(36, 2);						/*пермещение ввода в нижнюю область*/

	int ch;
	int position = 0;
	char message_total[MESSAGES_LEN_MAX];

	memset(message_total, 0, sizeof(message_total));

	int shm_id;
	if (!initscr())
	{
		fprintf(stderr, "[Клиент] Ошибка initscr\n");
		exit(1);
	}

	refresh();
	/*
	int width = 153;
	int height = 43;

	int offsetx = (COLS - width) / 2;
	int offsety = (LINES - height) / 2;
*/
	win_chat = newwin(35, 119, 0, 1);
	win_list = newwin(35, 30, 0, 121);
	win_mes = newwin(7, 150, 35, 1);

	box(win_chat, 0, 0);
	box(win_list, 0, 0);
	box(win_mes, 0, 0);

	wrefresh(win_chat);
	wrefresh(win_list);
	wrefresh(win_mes);

	/*получение идентификатора сегмента разделяемой памяти*/
	shm_id = shmget(777, sizeof(chat_data), 0666);
	if (shm_id < 0)
	{
		perror("[Клиент] Ошибка shmget");
		exit(EXIT_FAILURE);
	}

	/*подключение к сегменту разделяемой памяти*/
	chat = (chat_data *)shmat(shm_id, NULL, 0);
	if (chat == (chat_data *)(-1))
	{
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	memset(name_curent, 0, sizeof(name_curent));
	mvaddstr(36, 2, "[Введите имя пользователя]:");
	move(36, 27+2);

	while (((ch = getch()) != '\n') && (position < (50 - 1)))
	{
			name_curent[position++] = ch;
			name_curent[position] = '\0';
			mvwprintw(win_chat, 36, 27+2, "%s", name_curent);
	}
	
	pthread_mutex_lock(&chat->clients_mutex);
	strcpy(chat->client_in, name_curent);
	pthread_mutex_unlock(&chat->clients_mutex);
	wclear(win_mes);
	box(win_mes, 0, 0);
	wrefresh(win_mes);

	/*создание потока для чтения чата в фоново режиме*/
	if (pthread_create(&thread_chat_read, NULL, shm_read, NULL) != 0)
	{
		perror("[Клиент] Ошибка pthread_create");
		exit(EXIT_FAILURE);
	}

	move(36, 2); /*перемещение указателя в область для ввода текста*/

	while (1)
	{
		move(36, 2);
		position = 0;
		memset(buffer, 0, sizeof(buffer));

		while (((ch = getch()) != '\n') && (position < (MESSAGES_LEN_MAX - CLIENT_NAME_LEN - 3)) && (ch != 27))
		{
			if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') /*если нажали backspace*/
			{
				if (position > 0) /*проверка, что буфер не пустой*/
				{
					position--;
					buffer[position] = '\0';
				}
			}
			else
			{
				buffer[position++] = ch; /*сохранение символа в буфер*/
				buffer[position] = '\0'; /*установка конца строки*/
			}
			mvwprintw(win_chat, 36, 2, "%s", buffer); /*отображение введённого текста*/
		}

		if (ch==27) /*если нажали esc*/
		{
			break;
		}

		snprintf(message_total, sizeof(message_total), "%s: %s", name_curent, buffer);

		pthread_mutex_lock(&chat->messages_mutex);
		strcpy(chat->messages[chat->messages_count], message_total);
		chat->messages_count++;
		chat->version++;
		pthread_mutex_unlock(&chat->messages_mutex);
		wclear(win_mes);
		box(win_mes, 0, 0);
		wrefresh(win_mes);
	}
	pthread_mutex_lock(&chat->clients_mutex);
	strcpy(chat->client_out, name_curent);
	pthread_mutex_unlock(&chat->clients_mutex);

/*высвобождение ресурсов*/
	delwin(win_chat);
	delwin(win_list);
	delwin(win_mes);
	endwin();
	exit(EXIT_SUCCESS);
}
