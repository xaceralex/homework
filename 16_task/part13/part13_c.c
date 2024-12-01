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

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdint.h>

#include <sys/types.h>
#include <net/if.h>

#define ARPHRD_ETHER 1
#define PCKT_LEN 1024
#define ETH_LEN 14
#define IF_NAME "enp0s3" /*название интерфайса*/
#define IP_SADDR "192.168.1.218"
#define IP_DADDR "192.168.1.218"

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
	char *data;
	int sock;
	struct sockaddr_ll server_addr;
	int frame_len = 0;
	char src_mac[6] = {0x08,0x00,0x27,0x06,0x2a,0x8e}; /*mac-адрес отправителя*/
	char dst_mac[6] = {0x08,0x00,0x27,0x06,0x2a,0x8e}; /*mac-адрес получателя*/
	char eth_type[2];
	char message[256] = "Test"; //строка, которую будем отправлять
	char packet[PCKT_LEN];

	char frame[PCKT_LEN];
	memset(frame, 0, PCKT_LEN);
	
	//создание сокета
	if ((sock = socket(AF_INET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		perror("[Клиент] Ошибка socket");
		exit(EXIT_FAILURE);
	}
	
	//заполнение данных сервера
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sll_family = AF_INET;
	server_addr.sll_protocol = htons(ETH_P_IP);
	server_addr.sll_ifindex = if_nametoindex(IF_NAME);
	server_addr.sll_hatype = ARPHRD_ETHER;
	server_addr.sll_pkttype = PACKET_HOST;
	server_addr.sll_halen = ETH_ALEN;
	
	server_addr.sll_addr[0] = src_mac[0];
	server_addr.sll_addr[1] = src_mac[1];
	server_addr.sll_addr[2] = src_mac[2];
	server_addr.sll_addr[3] = src_mac[3];
	server_addr.sll_addr[4] = src_mac[4];
	server_addr.sll_addr[5] = src_mac[5];
	server_addr.sll_addr[6] = 0x00;
	server_addr.sll_addr[7] = 0x00;
	eth_type[0] = ETH_P_IP / 256;
	eth_type[1] = ETH_P_IP % 256;

	//заполнение указателей для заголовкой относительно пакета
	
	struct iphdr ip_header;
	struct udphdr udp_header;

	memset(&ip_header, 0, sizeof(struct iphdr));
	memset(&udp_header, 0, sizeof(struct udphdr));

	int total_length = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(message); //общая длина пакета
	frame_len = total_length + ETH_LEN;

	//заполнение ip-заголовка
	ip_header.version = 4;							//версия (у нас ipv4)
	ip_header.ihl = 5;								//длина заголовка ip
	ip_header.tos = 0;								//(type of service) определение типа/приоритета данных
	ip_header.tot_len = htons(total_length);		//общая длина
	ip_header.id = 0;								//идентификатор для фрагментации
	ip_header.ttl = 255;							//time to live/время жизни - количество прыжков
	ip_header.protocol = IPPROTO_UDP;				//ICMP/TCP/UDP
	ip_header.check = 0;							//контрольная сумма заголовка ip
	ip_header.saddr = inet_addr(IP_SADDR);	//ip-адрес отправителя
	ip_header.daddr = inet_addr(IP_DADDR);	//ip-адрес получателя

	//подсчёт и заполнение контрольной суммы для ip-заголовка
	ip_header.check = checksum((unsigned short *)packet, ip_header.tot_len);

	//заполнение udp-заголовка
	udp_header.source = htons(63122);
	udp_header.dest = htons(63123);
	udp_header.len = htons(sizeof(struct udphdr) + strlen(message));
	udp_header.check = 0; // Контрольная сумма UDP (можно оставить 0)

	//копирование сообщения после двух заголовков

	memcpy(frame, dst_mac, ETH_ALEN);
	memcpy(frame + ETH_ALEN, src_mac, ETH_ALEN);
	memcpy(frame + ETH_ALEN + ETH_ALEN, eth_type, sizeof(eth_type));

	data = frame + ETH_LEN;
	memcpy(data, &ip_header, sizeof(struct  iphdr));
	memcpy(data + sizeof(struct iphdr), &udp_header, sizeof(struct udphdr));
	memcpy(data + sizeof(struct iphdr) + sizeof(struct udphdr), message, strlen(message));

	//отправка пакета
	if (sendto(sock, frame, frame_len, 0,(struct sockaddr *) &server_addr, sizeof(struct sockaddr_ll)) < 0)
	{
		perror("[Клиент] Ошибка sendto");
		exit(EXIT_FAILURE);
	}

	close(sock); //закрытие сокета
	exit(EXIT_SUCCESS);
}
