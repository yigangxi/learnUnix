/*===============================================================
*   Copyright (C) 2018 All rights reserved.
*   
*   文件名称：Park_MS.c
*   创 建 者：yx32
*   创建日期：2018年11月09日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 3 //停车场所停车辆上限
#define STR_INIT "-1"
#define DATA_INIT -1

int day[3] = {365, 30, 1};
char seps[] = " -:";

typedef struct node{  //车信息结构体
	char num[10];
	int flag;
	char arrive_time[20];
}parknode;

int push(parknode *park, int *parktop, parknode *car){   //车辆进入处理
	if((*parktop) >= MAX){
		printf("停车场已停满！让该车停在便道上！\n");
		return -1;
	}
	else{
		park[(*parktop)] = *car;
		printf("该车停在%d号位\n", (*parktop)+1);
		(*parktop)++;
		return 1;
	}
}

//车辆离开处理函数
parknode pop(parknode *park, int *parktop, char carnum[], parknode *location, int *locationtop){
	int i;
	//初始化车辆
	parknode thecar;
	thecar.flag = DATA_INIT;
	strcpy(thecar.num, STR_INIT);
	strcpy(thecar.arrive_time, STR_INIT);
	//在停车场找到要出去的车
	for(i = 0; i < (*parktop); i++){
		if(strstr(park[i].num, carnum) != NULL)
			break;
	}
	//停车场无此车
	if(i == (*parktop)){
		printf("停车场无此车\n");
		thecar.flag = 0;
	}
	//查到该车，将该车移出停车场
	else{
		//将该车之后的车挪入另一个栈中
		while((*parktop) > i+1){
			(*parktop)--;
			location[*locationtop] = park[*parktop];
			(*locationtop)++;
		}
		(*parktop)--;  //栈顶指针下移一位
		thecar = park[(*parktop)];
		//该车出栈，将挪出的车全部进栈
		while((*locationtop) > 0){
			(*locationtop)--;
			park[(*parktop)] = location[(*locationtop)];
			(*parktop)++;
		}
	}
	return thecar;
}

parknode arrive_car(parknode *car, parknode *park, int *parktop, parknode *accessroad, int *front, int *rear){
	int flag_num = 0, i;
	struct tm *p;
	time_t timep;
	time(&timep);
	p = gmtime(&timep);
	printf("登记车牌号:");
	scanf("%s", car->num);
	while(1) {
		for ( i = 0; i < *parktop; i++ ) {
			if (strstr(car->num, park[i].num) != NULL) {
				flag_num = 1;
				break;
			}
		}
		if(flag_num == 1) {
			getchar();
			printf("输入的车牌号已存在！请重新输入:");
			scanf("%s", car->num);
			flag_num = 0;
		}
		else
			break;
	}
	//有车进入停车场
	snprintf(car->arrive_time, sizeof(car->arrive_time), "%d-%d-%d %d:%d:%d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, 8+p->tm_hour, p->tm_min, p->tm_sec);
	int result = push(park, parktop, car);
	//如果返回值为-1，证明停车场已满，需要停在便道上
	if(result == -1){
		accessroad[*rear] = *car;
		printf("该车在便道上%d号位\n", (*rear) + 1 - (*front));
		(*rear)++;
	}
	else{
		printf("车辆到达时间:%s\n", car->arrive_time);
		strcpy(car->arrive_time, STR_INIT);
	}
	return *car;
}

int caculate_time(parknode *car){
	int times = 0, ptime[5], i = 0, len = 0;
	char *str_temp;
	struct tm *pt;
	time_t timep;
	time(&timep);
	pt = gmtime(&timep);
	ptime[0] = 1900 + pt->tm_year;
	ptime[1] = 1 + pt->tm_mon;
	ptime[2] = pt->tm_mday;
	ptime[3] = 8 + pt->tm_hour;
	ptime[4] = pt->tm_min;
	printf("%d-%d-%d %d:%d:%d\n", 1900+pt->tm_year, 1+pt->tm_mon, pt->tm_mday, 8+pt->tm_hour, pt->tm_min, pt->tm_sec);
	str_temp = strtok(car->arrive_time, seps);
	for(i = 0; i < 5; i++){
		len = ptime[i] - atoi(str_temp);
		if(i < 3)
			times += day[i] * len * 24;
		else if(i == 3)
			times += len;
		else{
			if(len > 0)
				times++;
		}
		str_temp = strtok(NULL, seps);
	}
	return times;
}

parknode leave_car(parknode *car, parknode *park, int *parktop, parknode *accessroad, int *front, int *rear){
	char carnum[10]; //车辆车牌号
	int times;
	//辅助停车场进行挪车的栈
	parknode location[MAX];
	int locationtop = 0;  //栈顶指针

	printf("离开停车场的车的车牌号:");
	scanf("%s", carnum);
	//当有车辆需要出停车场时，调用出栈函数
	*car = pop(park, parktop, carnum, location, &locationtop);
	//如果返回的车牌号为-1，即停车场无此车
	if(car->flag != 0){
		printf("该车离开停车场的时间:");
		times = caculate_time(car);//停留时间
		printf("该车停留时间(不足一小时按一小时计算):%d小时，应缴费:%.2f元\n", times, times*1.5);
		//若便道上有车等待，便可进入停车场，进入时设定进入时间
		if((*front) != (*rear)){
			(*car) = accessroad[*front];
			printf("便道上车牌号为%s的车进入时间为:", car->num);
			struct tm *p;
			time_t timep;
			time(&timep);
			p = gmtime(&timep);
			printf("%d-%d-%d %d:%d:%d\n", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, 8+p->tm_hour, p->tm_min, p->tm_sec);
			snprintf(car->arrive_time, sizeof(car->arrive_time), "%d-%d-%d %d:%d:%d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, 8+p->tm_hour, p->tm_min, p->tm_sec);
			park[*parktop] = *car;
			(*front)++;
			(*parktop)++;
		}
		else
			printf("便道上没有车等待，停车场有空余位置\n");
		return *car;
	}
}

int main(){
	//停车场的栈
	parknode park[MAX];
	int parktop = 0;   //栈顶指针

	//模拟便道队列
	parknode accessroad[10];
	int front = 0, rear = 0;  //队头和队尾指针
	char order; //进出停车场的指令
	printf("有车辆进入停车场(A)；有车辆离开停车场(D)；程序停止(#):");
	parknode car;
	while(1 == scanf("%c", &order)){
		while(1){
			if(order == 'A' || order == 'D' || order == '#')
				break;
			else{
				getchar();
				printf("输入错误！请重新输入:");
				scanf("%c", &order);
			}
		}
		if(order == '#')
			break;
		switch(order){
			case 'A':
				arrive_car(&car, park, &parktop, accessroad, &front, &rear);
				break;
			case 'D':
				leave_car(&car, park, &parktop, accessroad, &front, &rear);
				break;
			default:
				break;
		}
		printf("有车辆进入停车场(A)；有车辆离开停车场(D)；程序停止(#):");
		getchar();//清空缓冲区
	}
	return 0;
}
