#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

struct packet{
	int len;
	char buf[BUFSIZ];
};

ssize_t readn(int fd, void *buf, size_t count){
	size_t nleft=count;
	ssize_t nread;
	char *bufp = (char *)buf;
	while(nleft > 0){
		if((nread = read(fd,bufp,nleft)) < 0){
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nread == 0)
			return count - nleft;
		bufp += nread;
		nleft -= nread;
	}
	return count;
}

ssize_t writen(int fd, const void *buf, size_t count){
	size_t nleft =  count;
	ssize_t nwrite;
	char *bufp = (char *)buf;
	while(nleft > 0){
		if((nwrite = write(fd,bufp,nleft)) < 0){
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nwrite == 0)
			continue;
		bufp += nwrite;
		nleft -= nwrite;
	}
	return count;
}

int main(void){
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

	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf,0,sizeof(sendbuf));
	memset(&recvbuf,0,sizeof(recvbuf));
	int n;
	while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin) != NULL){
		n = strlen(sendbuf.buf);
		sendbuf.len = htonl(n);
		writen(client_sockfd,&sendbuf,4+n);

		int ret = readn(client_sockfd,&recvbuf.len,4);
		if(ret == -1){
			perror("read error");
			return -1;
		}
		else if(ret < 4){
			printf("client close\n");
			break;
		}
		n = ntohl(recvbuf.len);
		ret = readn(client_sockfd,recvbuf.buf,n);	
		if(ret == -1){
			perror("read error");
			return -1;
		}
		else if(ret < n){
			printf("client close\n");
			break;
		}

		fputs(recvbuf.buf,stdout);
		memset(&recvbuf,0,sizeof(recvbuf));
		memset(&sendbuf,0,sizeof(sendbuf));
	}

	//the forth,close taojiezi
	close(client_sockfd);
	
	return 0;
}
