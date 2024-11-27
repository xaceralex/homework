/*
*	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PCKT_LEN 8192

//вычисление контрольной суммы
unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b; //пакет
	unsigned int sum = 0; //сумма
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;

	return result;
}

int main()
{
	int sock;
	struct sockaddr_in server_addr;
	char *message = "Test"; //строка, которую будем отправлять
	char packet[PCKT_LEN];

	//создание сокета
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
	{
		perror("[Клиент] Ошибка socket");
		exit(EXIT_FAILURE);
	}

	//заполнение данных сервера
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(63123);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.133"); // Замените на адрес вашего сервера

	//заполнение указателей для заголовкой относительно пакета
	struct iphdr *ip_header = (struct iphdr *)packet;
	struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct iphdr));

	int total_length = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message); //общая длина пакета

	//заполнение ip-заголовка
	ip_header->version = 4;							//версия (у нас ipv4)
	ip_header->ihl = 5;								//длина заголовка ip
	ip_header->tos = 0;								//(type of service) определение типа/приоритета данных
	ip_header->tot_len = htons(total_length);		//общая длина
	ip_header->id = 0;								//идентификатор для фрагментации
	ip_header->ttl = 255;							//time to live/время жизни - количество прыжков
	ip_header->protocol = IPPROTO_UDP;				//ICMP/TCP/UDP
	ip_header->check = 0;							//контрольная сумма заголовка ip
	ip_header->saddr = inet_addr("192.168.1.218");	//ip-адрес отправителя
	ip_header->daddr = server_addr.sin_addr.s_addr;	//ip-адрес получателя

	//подсчёт и заполнение контрольной суммы для ip-заголовка
	ip_header->check = checksum((unsigned short *)packet, ip_header->tot_len);

	//заполнение udp-заголовка
	udp_header->source = htons(63122);
	udp_header->dest = htons(63123);
	udp_header->len = htons(sizeof(struct udphdr) + strlen(message));
	udp_header->check = 0; // Контрольная сумма UDP (можно оставить 0)

	//копирование сообщения после двух заголовков
	memcpy(packet + sizeof(struct iphdr) + sizeof(struct udphdr), message, strlen(message));

	//отправка пакета
	if (sendto(sock, packet, total_length, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("[Клиент] Ошибка sendto");
		exit(EXIT_FAILURE);
	}

	close(sock); //закрытие сокета
	exit(EXIT_SUCCESS);
}
