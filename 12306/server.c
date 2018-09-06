#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void do_server(int client_sockfd){
	char recvbuf[BUFSIZ];
	while(1){
    	memset(recvbuf,0,sizeof(recvbuf));
        int ret = read(client_sockfd,recvbuf,sizeof(recvbuf));
		if(ret == 0){
			printf("client close\n");
			break;
		}
		else if(ret ==-1){
			perror("read error");
			return -1;
		}
        fputs(recvbuf,stdout);
		write(client_sockfd,recvbuf,ret);
	 }
}

int main(int argc, char *argv[]){
	//build taojiezi
	int server_sockfd;//server taojiezi
	if((server_sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0){
		perror("socket error");
		return -1;
	}

	int opt = 1;
	if(setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0){
		perror("setsockopt error");
	}

	//bind ip port ipv4
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8080);
	if(bind(server_sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
		perror("bind error");
		return -1;
	}

	//listen 5
	if(listen(server_sockfd,5) < 0){
		perror("listen error");
		return -1;
	}

	//connect 
	struct sockaddr_in client_addr;
	int client_sockfd;
	socklen_t length;
	length = sizeof(struct sockaddr_in);

	pid_t pid;
	while(1){
		if((client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr,&length)) < 0){
			perror("accept error");
			return -1;
		}
		printf("connected success!\n");
		printf("ip = %s\nport = %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		pid = fork();
		if(pid == -1){
			perror("fork error");
			return -1;
		}
		if(pid == 0){
			close(server_sockfd);
			do_server(client_sockfd);
			exit(1);
		}
		else
			close(client_sockfd);
	}
	return 0;
}
