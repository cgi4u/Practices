#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int sockfd; // socket descripter
	int acc_sock;
    struct sockaddr_in servaddr, cliaddr;
	int addrlen = sizeof(cliaddr);

    if(argc < 2)
    {
        printf("Usage: %s port \n", argv[0]);
        return -1;
    }

    // PF: Protocol Family, AF: Address Family

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket failed !");
        return -1;
    }

    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		perror("bind failed!");
		return -1;
	}

	listen(sockfd, 3);	// 인자2는 동시접속자수

	while ( 1 ){
		puts("Listening...");
		acc_sock = accept(sockfd, (struct sockaddr*)&cliaddr, (socklen_t*)&addrlen);

		if (acc_sock < 0){
			perror("accept failed!");
			return -1;
		}

		puts("Connected client");
	}

	close(sockfd);
    return 0;
}
