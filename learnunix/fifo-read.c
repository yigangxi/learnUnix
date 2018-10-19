#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define _PATH_ "/home/yx32/learnunix/file.fifo"

int main(){
	int fd = open(_PATH_, O_RDONLY);//以只读的权限打开文件
	if(fd < 0){
		printf("打开失败\n");
		return 2;
	}
	char recv[BUFSIZ];
	while(1){
		memset(recv, 0, sizeof(recv));//字符串清零
		int ret = read(fd, recv, sizeof(recv));
		if(strstr(recv, "quit") != 0){//判断接收到的数据是否是quit
			printf("退出程序\n");
			exit(0);
		}
		printf("message:%-20s. length:%d\n", recv, ret);
	}

	close(fd);//关闭文件
	return 0;
}
