#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <string.h>

int main(){
	int _pipe[2];
	int ret = pipe(_pipe);//类似两个文件指针，针对管道中的读(0)和写(1)
	if(ret == -1){
		printf("创建管道失败，错误码是:%d,%d\n",error,strerror(error));
		return 1;
	}

	pid_t pid = fork();//创建子进程

	if(pid < 0){
		printf("创建子进程错误\n");
		return 2;
	}

	else if(pid == 0){  //子进程的操作
		close(_pipe[0]);  // 关闭读通道
		int i = 0;
		char mesg[1024];
		while( i < 20){
			scanf("%s", mesg);
			write(_pipe[1], mesg, strlen(mesg));
			sleep(2);
			i++;
		}
	}
	else{    //进入父进程，从子进程读取数据
		close(_pipe[1]);  //关闭写通道
		char _mesg[1024];
		int j = 0;
		while(j < 20){
			memset(_mesg, 0, sizeof(_mesg));
			int ret = read(_pipe[0], _mesg, sizeof(_mesg));
			if(strstr(_mesg, "quit") != 0){
				printf("退出程序\n");
				exit(0);
			}
			printf("message:%s. length:%d. count:%d\n", _mesg, ret, ++j);
		}

		if(waitpid(pid, NULL, 0) < 0){  //避免父进程与子进程冲突，让子进程先执行
			return 3;
		}
	}
	return 0;
}
