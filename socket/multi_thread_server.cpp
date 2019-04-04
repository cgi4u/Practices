#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

using namespace std;

mutex m;
bool avail_thread[3] = { true, true, true };

struct arg_struct {
	int acc_sock;
	int thread_idx;
};

void *cli_thread(void *arg){
	struct arg_struct *args; 
	args = (struct arg_struct*)arg;

	int acc_sock = args->acc_sock;
	int thread_idx = args->thread_idx;
	char buffer[300];

	printf("Thread with index %d successfully initialzed\n", thread_idx);

	while (1){
		bzero(buffer, 301);

		read(acc_sock, buffer, 300);
		printf("%s\n", buffer);
		
		if (strcmp(buffer, "exit") == 0)
			break;
	}

	printf("Close socket %d", acc_sock);
	close(acc_sock);

	m.lock();
	avail_thread[thread_idx] = true;
	m.unlock();
};

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
        perror("Socket failed!");
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

	pthread_t cli_threads[3];
	bool is_thread_allocated[3] = { false, false, false };

	while ( 1 ){
		puts("Listening...");
		acc_sock = accept(sockfd, (struct sockaddr*)&cliaddr, (socklen_t*)&addrlen);

		if (acc_sock < 0){
			perror("accept failed!");
			return -1;
		}

		puts("Connected client");
		
		bool created = false;
		for (int i = 0; i < 3; i++){
			if (!avail_thread[i]) continue;

			if (is_thread_allocated[i])
				pthread_join(cli_threads[i], NULL);

			struct arg_struct args;
			args.acc_sock = acc_sock;
			args.thread_idx = i;

			if (pthread_create(&cli_threads[i], NULL, cli_thread, (void*)&args)){
				perror("thread creation failed!");
				break;
			}

			is_thread_allocated[i] = true;
			created = true;

			m.lock();
			avail_thread[i] = false;
			m.unlock();

			break;
		}

		if (!created)
			printf("There is no available thread slot.\n");
	}

	close(sockfd);
    return 0;
}
