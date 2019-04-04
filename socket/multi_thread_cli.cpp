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

	int check = connect(sock_cli, (struct sockaddr*)&sock_serv_addr, sizeof(sock_serv_addr));
	if (check < 0) {
		perror("Connection failed!");
		return -1;
	}

	puts("Connection Success!");
	
	char buffer[300];
	while (1) {
		fgets(buffer, 300, stdin);
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';

		int temp = send(sock_cli, buffer, strlen(buffer), 0);
		
		if (strcmp(buffer, "exit") == 0)
			break;
	}

	closesocket(sock_cli);
	WSACleanup();

	return 0;
}