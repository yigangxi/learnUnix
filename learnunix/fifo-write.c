#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define _PATH_ "/home/yx32/learnunix/file.fifo"

int main(){
	int ret = mkfifo(_PATH_, 0666);//创建管道文件
	if(ret == -1){
		printf("创建管道失败\n");
		exit(0);
	}
	int fd = open(_PATH_, O_WRONLY);//以只写的权限打开文件
	if(fd < 0){
		printf("打开失败\n");
		exit(0);
	}
	char buf[BUFSIZ];
	while(1){
		memset(buf, 0, strlen(buf));//字符串清零
		printf("please input the string:");
		scanf("%s", buf);
		int ret = write(fd, buf, strlen(buf));//写数据
		if(ret < 0){
			printf("写出错\n");
			break;
		}
		if(strstr(buf, "quit") != 0){
			break;
		}
	}

	close(fd);//关闭文件
	unlink(_PATH_);//删除管道文件
	return 0;
}
