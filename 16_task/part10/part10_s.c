/*
*	Эта программа выступает в роли сервера. Сервер
*	использует мультиплексирование, поэтому он может
*	принимать сообщения от клиентов использующих 
*	как протокол TCP так и протокол UDP.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 7777
#define EVENTS_MAX 10
#define SIZE 256

int main()
{
	int tcp_sock, udp_sock, epoll_fd;
	struct sockaddr_in address;
	char buffer[SIZE];
	int nfds;
	int addrlen;
	int new_socket;
	int valread;
	int len;


	//создание сокета для tcp
	if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("[Сервер] Ошибка socket(tcp)");
		exit(EXIT_FAILURE);
	}

	//создание сокета для udp
	if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("[Сервер] Ошибка socket(udp)");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//привязка tcp сокета, переход в режим прослушивания 
	bind(tcp_sock, (struct sockaddr *)&address, sizeof(address));
	listen(tcp_sock, 5);

	//привязка udp сокета
	bind(udp_sock, (struct sockaddr *)&address, sizeof(address));

	//создание epoll
	if ((epoll_fd = epoll_create1(0)) < 0)
	{
		perror("[Сервер] Ошибка epoll_create1");
		exit(EXIT_FAILURE);
	}

	//заполнение структуры epoll для tcp
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = tcp_sock;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_sock, &ev);

	//заполнение структуры epoll для udp
	ev.data.fd = udp_sock;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_sock, &ev);

	printf("[Сервер] Идёт прослушивание на порту: %d\n", PORT);

	while (1)
	{
		struct epoll_event events[EVENTS_MAX];
		nfds = epoll_wait(epoll_fd, events, EVENTS_MAX, -1);
		
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == tcp_sock)
			{
				addrlen = sizeof(address);
				new_socket = accept(tcp_sock, (struct sockaddr *)&address, (socklen_t*)&addrlen);
				valread = read(new_socket, buffer, SIZE);
				buffer[valread] = '\0';
				printf("[Сервер] Получено сообщение от TCP сервера: %s\n", buffer);
				close(new_socket);
			}
			else
			if (events[i].data.fd == udp_sock)
			{
				socklen_t addrlen = sizeof(address);
				len = recvfrom(udp_sock, buffer, SIZE, 0, (struct sockaddr *)&address, &addrlen);
				buffer[len] = '\0';
				printf("[Сервер] Получено сообщение от UDP сервера: %s\n", buffer);
			}
		}
	}

	close(tcp_sock);
	close(udp_sock);
	close(epoll_fd);
	exit(EXIT_SUCCESS);
}