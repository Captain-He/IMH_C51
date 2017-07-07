#include<stdio.h>
#include<string.h>
#include<reg52.h>

//数据包分解函数
void tcp(unsigned char* sjb)//接收到数据包数据,如果是字符串型
{
	int i;
	int n = strlen(sjb);
	unsigned char bao[50];
	unsigned char fen[] = ":,";
	unsigned char *head = NULL;//IMH
	unsigned char *lei = NULL;//01234
	unsigned char *id = NULL;//"id"
	unsigned char *id_detail = NULL;//id
	unsigned char *name = NULL;//"name"
	unsigned char *name_detail = NULL;//name
	unsigned char *color = NULL;//"color"
	unsigned char *color_detail = NULL;//color
	unsigned char *time = NULL;//"time"
	unsigned char *hour = NULL;//hour
	unsigned char *minute = NULL;//minute
	unsigned char *fuyao = NULL;//"eat"
	unsigned char *fuyao_detail = NULL;//eat
	unsigned char *yanshi = NULL;//"out"
	unsigned char *yanshi_detail = NULL;//out

	//把接收到的字符串转换成数组
	for(i = 0; i < n+1; i++)
	{
		bao[i] =*(sjb++);
	}
	
	//字符串分割
	head = strtok(bao,fen);
	lei = strtok(NULL,fen);
	id = strtok(NULL,fen);
	id_detail = strtok(NULL,fen);

	/*
	name = strtok(NULL,fen);
	name_detail = strtok(NULL,fen);
	color = strtok(NULL,fen);
	color_detail = strtok(NULL,fen);
	time = strtok(NULL,fen);
	hour = strtok(NULL,fen);
	minute = strtok(NULL,fen);

	fuyao = strtok(NULL,fen);
	fuyao_detail = strtok(NULL,fen);
	yanshi = strtok(NULL,fen);
	yanshi_detail = strtok(NULL,fen);
	*/

	//如果包头是IMH
	if(!strcmp(head,"IMH"))
	{
		//控制类0
		if(!strcmp(lei,"0"))
		{
			//字符串分割
			name = strtok(NULL,fen);
			name_detail = strtok(NULL,fen);
			color = strtok(NULL,fen);
			color_detail = strtok(NULL,fen);
			time = strtok(NULL,fen);
			hour = strtok(NULL,fen);
			minute = strtok(NULL,fen);

			//id
			if(!strcmp(id,"id")){}
			//color按红绿蓝的顺序
			if(!strcmp(color,"color"))
			{
				if(!strcmp(color_detail,"100")){}
				if(!strcmp(color_detail,"010")){}
				if(!strcmp(color_detail,"001")){}
			}
			//time
			if(!strcmp(time,"time")){}

		}
		//返回类1
		if(!strcmp(lei,"1"))
		{
			//添加返回信息的函数
			/*SBUF = rt;
			while(!TI);
			TI = 0;*/
		}
		//查询类2
		if(!strcmp(lei,"2"))
		{
			//添加查询id的函数
		}
		//数据类3
		if(!strcmp(lei,"3"))
		{
			//字符串分割
			name = strtok(NULL,fen);
			name_detail = strtok(NULL,fen);
			color = strtok(NULL,fen);
			color_detail = strtok(NULL,fen);
			time = strtok(NULL,fen);
			hour = strtok(NULL,fen);
			minute = strtok(NULL,fen);

			//color按红绿蓝的顺序
			if(!strcmp(color,"color"))
			{
				if(!strcmp(color_detail,"100")){}//添加控制灯的函数
				if(!strcmp(color_detail,"010")){}
				if(!strcmp(color_detail,"001")){}
			}
			//time
			if(!strcmp(time,"time")){}//添加时间的函数hour和minute为字符串类型
		}
		//状态类4
		if(!strcmp(lei,"4"))
		{
			//字符串分割
			fuyao = strtok(NULL,fen);
			fuyao_detail = strtok(NULL,fen);
			yanshi = strtok(NULL,fen);
			yanshi_detail = strtok(NULL,fen);
			//eat
			if(!strcmp(fuyao,"eat"))//添加服药状态的函数
			{
				if(!strcmp(fuyao_detail,"0")){}
				if(!strcmp(fuyao_detail,"1")){}	
			}
			//out
			if(!strcmp(yanshi,"out")){}//添加服药延时的函数
		}
	}
}