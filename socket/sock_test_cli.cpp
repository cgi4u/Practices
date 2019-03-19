#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

int main(void) {
	WSADATA wsa_data;
	SOCKET sock_cli;
	struct sockaddr_in sock_serv_addr;
	unsigned long addr_temp;

	WSAStartup(MAKEWORD(2, 0), &wsa_data);

	sock_cli = socket(AF_INET, SOCK_STREAM, 0);

	sock_serv_addr.sin_family = AF_INET;
	sock_serv_addr.sin_port = htons(1536);
	InetPton(AF_INET, "52.79.76.143", &addr_temp);
	sock_serv_addr.sin_addr.S_un.S_addr = addr_temp;

	int temp = connect(sock_cli, (struct sockaddr*)&sock_serv_addr, sizeof(sock_serv_addr));
	if (temp < 0) {
		perror("Connection failed!");
		return -1;
	}

	puts("Connection Success!");
	closesocket(sock_cli);

	WSACleanup();

	return 0;
}