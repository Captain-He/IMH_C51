#include<stdio.h>
#include<string.h>
#include<reg52.h>

//���ݰ��ֽ⺯��
void tcp(unsigned char* sjb)//���յ����ݰ�����,������ַ�����
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

	//�ѽ��յ����ַ���ת��������
	for(i = 0; i < n+1; i++)
	{
		bao[i] =*(sjb++);
	}
	
	//�ַ����ָ�
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

	//�����ͷ��IMH
	if(!strcmp(head,"IMH"))
	{
		//������0
		if(!strcmp(lei,"0"))
		{
			//�ַ����ָ�
			name = strtok(NULL,fen);
			name_detail = strtok(NULL,fen);
			color = strtok(NULL,fen);
			color_detail = strtok(NULL,fen);
			time = strtok(NULL,fen);
			hour = strtok(NULL,fen);
			minute = strtok(NULL,fen);

			//id
			if(!strcmp(id,"id")){}
			//color����������˳��
			if(!strcmp(color,"color"))
			{
				if(!strcmp(color_detail,"100")){}
				if(!strcmp(color_detail,"010")){}
				if(!strcmp(color_detail,"001")){}
			}
			//time
			if(!strcmp(time,"time")){}

		}
		//������1
		if(!strcmp(lei,"1"))
		{
			//��ӷ�����Ϣ�ĺ���
			/*SBUF = rt;
			while(!TI);
			TI = 0;*/
		}
		//��ѯ��2
		if(!strcmp(lei,"2"))
		{
			//��Ӳ�ѯid�ĺ���
		}
		//������3
		if(!strcmp(lei,"3"))
		{
			//�ַ����ָ�
			name = strtok(NULL,fen);
			name_detail = strtok(NULL,fen);
			color = strtok(NULL,fen);
			color_detail = strtok(NULL,fen);
			time = strtok(NULL,fen);
			hour = strtok(NULL,fen);
			minute = strtok(NULL,fen);

			//color����������˳��
			if(!strcmp(color,"color"))
			{
				if(!strcmp(color_detail,"100")){}//��ӿ��ƵƵĺ���
				if(!strcmp(color_detail,"010")){}
				if(!strcmp(color_detail,"001")){}
			}
			//time
			if(!strcmp(time,"time")){}//���ʱ��ĺ���hour��minuteΪ�ַ�������
		}
		//״̬��4
		if(!strcmp(lei,"4"))
		{
			//�ַ����ָ�
			fuyao = strtok(NULL,fen);
			fuyao_detail = strtok(NULL,fen);
			yanshi = strtok(NULL,fen);
			yanshi_detail = strtok(NULL,fen);
			//eat
			if(!strcmp(fuyao,"eat"))//��ӷ�ҩ״̬�ĺ���
			{
				if(!strcmp(fuyao_detail,"0")){}
				if(!strcmp(fuyao_detail,"1")){}	
			}
			//out
			if(!strcmp(yanshi,"out")){}//��ӷ�ҩ��ʱ�ĺ���
		}
	}
}