#include "part3_lib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int Socket(int domain, int type, int protocol)
{
	int res = socket(domain, type, protocol);
	if (res == -1)
	{
		perror("[Общ] Ошибка socket");
		exit(EXIT_FAILURE);
	}
	return (res);
}

void Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
	int res = bind(sockfd, addr, addrlen);
	if (res == -1)
	{
		perror("[Сервер] Ошибка bind");
		exit(EXIT_FAILURE);
	}
}

void Listen(int sockfd, int backlog)
{
	int res = listen(sockfd, backlog);
	if (res == -1)
	{
		perror("[Сервер] Ошибка listen\n");
		exit(EXIT_FAILURE);
	}
}

int Accept (int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int res = accept(sockfd, addr, addrlen);
	if (res == -1)
	{
		perror("[Сервер] Ошибка accept\n");
		exit(EXIT_FAILURE);
	}
	return (res);
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int res = connect(sockfd, addr, addrlen);
	if (res == -1)
	{
		perror("[Клиент] Ошибка connect\n");
		exit(EXIT_FAILURE);
	}
}

void Inet_pton(int af, const char *src, void *dst)
{
	int res = inet_pton(af, src, dst);
	if (res == 0) 
	{
		printf("[Клиент] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}
	if (res == -1) 
	{
		perror("[Клиент] Ошибка inet_pton\n");
		exit(EXIT_FAILURE);
	}

}