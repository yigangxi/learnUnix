/*===============================================================
*   Copyright (C) 2018 All rights reserved.
*   
*   文件名称：OLNode.c
*   创 建 者：yx32
*   创建日期：2018年11月22日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int location_row;  //所在行位置
	int location_col;  //所在列位置
	int val;           //值
}triple;

typedef struct {
	triple data[10];
	int row;   //矩阵的行
	int col;   //矩阵的列
	int num;   //矩阵中非0数
}TSMatrix;

void Fast(TSMatrix M, TSMatrix T) {
	int array[3] = { 0 };	//存储每一列的非0值
	int cpot[4];			//存储每一列第一个非0值在转置后的矩阵中的位置
	int i, j, k;
	T.col = M.row;
	T.row = M.col;
	T.num = M.num;
	for(i = 0; i < T.num; i++) {
		j = M.data[i].location_col;
		array[j]++;
	}

	cpot[1] = 1;
	for(i = 2; i <= M.col; i++) {
		cpot[i] = cpot[i-1] + array[i-1];
	}

	for(i = 0; i < T.num; i++) {
		j = M.data[i].location_col;
		k = cpot[j];
		T.data[k-1].location_col = M.data[i].location_row;
		T.data[k-1].location_row = M.data[i].location_col;
		T.data[k-1].val = M.data[i].val;
		cpot[j]++;
	}

	for(i = 0; i < T.num; i++)
		printf("T.data[%d].location_row:%d\nT.data[%d].location_col:%d\nT.data[%d].val:%d\n", 
				i, T.data[i].location_row, i, T.data[i].location_col, i, T.data[i].val);
}

int main(){
	TSMatrix M;
	TSMatrix T;
	M.row = 3;
	M.col = 2;
	M.num = 4;

	M.data[0].location_row = 1;
	M.data[0].location_col = 2;
	M.data[0].val = 1;

	M.data[1].location_row = 2;
	M.data[1].location_col = 1;
	M.data[1].val = 6;
	
	M.data[2].location_row = 2;
	M.data[2].location_col = 2;
	M.data[2].val = 3;

	M.data[3].location_row = 3;
	M.data[3].location_col = 2;
	M.data[3].val = 5;

	Fast(M, T);
	return 0;
}
