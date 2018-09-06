#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	//the first,build taojiezi
	int client_sockfd;
	if((client_sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0){
		perror("socket error");
		return -1;
	}

	//the second,bindtaojiezi
	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("192.168.10.128");
	client_addr.sin_port = htons(8080);
	if(connect(client_sockfd,(struct sockaddr *)&client_addr,sizeof(struct sockaddr)) < 0){
		perror("connect error");
		return -1;
	}
	printf("connected to server\n");

	//the third,shujujiaohu
	char sendbuf[BUFSIZ];
	char recvbuf[BUFSIZ];
	while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL){
		write(client_sockfd,sendbuf,strlen(sendbuf));
		read(client_sockfd,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
		memset(recvbuf,0,sizeof(recvbuf));
	}

	//the forth,close taojiezi
	close(client_sockfd);
	
	return 0;
}
