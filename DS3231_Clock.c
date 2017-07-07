#include<reg52.h> //51��Ƭ�����õ�ͷ�ļ������峣�õ�IO��
#include "DS3231.h" //DS3231ģ���Զ���ͷ�ļ�
#include "LedSegmentDisplay.h" //�������ʾ�Զ���ͷ�ļ�
#include "Led.h" //LED���Զ���ͷ�ļ�
#include "Protect.h"
#include "HCSR04.h" //������ͷ�ļ�


void DS3231_main() ;
void time_go();

#define uint unsigned int 
#define uchar unsigned char

sbit sound = P2^3;    //������IO��
//sbit play  = P1^4;    //ISD1820���β���IO��
//sbit led=P1^3;
bit flag2=0;
uchar temp,hour,minute,second,hour1,minute1,second1;
uchar RXDdata[50] = {0};
uint count = 0,m = 0;
uint a,t,v,h,num = 0,numb = 0;
uchar code table[] = { //�������ʾ��
		0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};

DS3231_TIME CurrentTime; //DS3231ʱ���Զ���ʱ��ṹ��


void sendchar(uchar byte)
{
	SBUF = byte;
	while(!TI);
	TI = 0;
}
void sendString(uchar* s)
{
	while(*s!='\0')
	{
		sendchar(*s);
		s++;
	}
}

void initEsp()
{
	SCON = 0x50; //8λ���ݣ��ɱ䲨����
	TMOD = 0x20; //���ö�ʱ��1λ16λ�Զ���װģʽ
	TL1  = 0xfd; //���ö�ʱ����ֵ
	TH1  = 0xfd;
	ET1  = 0; //��ֹ��ʱ��1�ж�
	TR1  = 1; //������ʱ��1
	EA   = 1;
	for(a = 0; a < 10000; a++); //��ʱ���룬��ģ����ʱ������
	sendString("AT+CWMODE=2\r\n"); //����ΪsoftAPģʽ
	/*
	for(a = 0; a < 1000; a++); //��ʱ���룬��ģ����ʱ������
	sendString("AT+RST\r\n");
	*/ 
	for(a = 0; a < 10000; a++); 
	sendString("AT+CIPMUX=1\r\n"); //����������

	for(a = 0; a < 10000; a++);
	sendString("AT+CIPSERVER=1,8888\r\n");//����server��Ĭ�϶˿�Ϊ8888
	for(a = 0; a < 10000; a++); 
	RI=0;
	ES=1; //��ʼ����ɣ����п��жϴ�
}

void getData()
{
	if(flag2 == 1)
	{ 
		for(t = 0; t < 50; t++)
		{
			sendchar(RXDdata[t]);
			for(a = 0; a < 1000; a++); 
		}
		sendchar('\n');

		while(RXDdata[numb]!='@')
		{
			if(RXDdata[numb]==' ') num=num+1;
			if(num==4)
			   h=numb;
	  		if(num==10)      //numb ����        
			{                // v ʱ��ǰ�Ǹ��ո����������
	     		v=numb;		//num�ո�ĸ��� 
			 	RXDdata[numb]='@';
			 	numb--;
		 	}
	 		numb++;
		}
		if(RXDdata[h]=='1')
		{
		  hour=(RXDdata[v+1]-'0')*10+(RXDdata[v+2]-'0');
          minute=(RXDdata[v+4]-'0')*10+(RXDdata[v+5]-'0');
	      second=(RXDdata[v+7]-'0')*10+(RXDdata[v+8]-'0');	 
		}
	//	 sendchar(RXDdata[h]);
	   if(RXDdata[h]=='2')
		{
		 hour1=(RXDdata[v+1]-'0')*10+(RXDdata[v+2]-'0');
          minute1=(RXDdata[v+4]-'0')*10+(RXDdata[v+5]-'0');	
		  second1=(RXDdata[v+7]-'0')*10+(RXDdata[v+8]-'0');
		} 
	
		num = 0;
		v=0;
		h=0;
		numb=0;

		SectorErase(0x2000);//��������
		byte_write(0x2000,hour);//����д������
        byte_write(0x2010,minute);
        byte_write(0x2020,second);

		flag2 = 0;
		count = 0;
		for(m=0;m<50;m++)
		{
			RXDdata[m] = '*';	
		}
	}
}

void main()
{

	initEsp();
	//led=1;
	DS3231_main();

	count =0;
	flag2 = 0;

	for(m=0;m<50;m++)
	{
		RXDdata[m] = '*';

	}
	led_off();
	led_off1();
	while(1)
	{ 
		getData();
		time_go();
	}
}

void SISR(void) interrupt 4
{ 
	if(RI == 1)
	{
		ES = 0; //�رմ����ж�
		RI = 0; //������н��ܱ�־λ
		temp =SBUF; //�Ӵ��ڻ�����ȡ������
		if(count<50)
		{ 
			RXDdata[count]=temp;
			count++;
			if(RXDdata[0]=='+')
			{
			   if(temp=='@')
			   {
			   		flag2 = 1;
					count = 50;
				} 
			}
			else
			{
				flag2 = 0;
				count = 0;
			}
		// if(temp=='@'||temp=='\0'||count>=20)
		// flag=1;
		} 
	ES = 1; //�������ж�
	}
}
/********* *****************************/




void warn(uchar n) //����������
{
	uchar i;
	for(i = 0; i < n; i++)
	{
		sound = 0;
		delay(20);
		sound = 1; 
	}
}

void AlarmClock(uchar hour, uchar minute, uchar second,uchar hour1, uchar minute1,uchar second1) //��������
{
	uchar clock[5];
	DS3231WriteData(DS3231_ALARM1HOUR, hour); //д������ʱ��
	DS3231WriteData(DS3231_ALARM1MINUTE, minute);
	DS3231WriteData(DS3231_ALARM1SECOND, second);
	clock[0] = DS3231ReadData(DS3231_ALARM1HOUR); //��ȡ����ʱ��
	clock[1] = DS3231ReadData(DS3231_ALARM1MINUTE);
	clock[2] = DS3231ReadData(DS3231_ALARM1SECOND);

	DS3231WriteData(DS3231_ALARM2HOUR, hour1); //д������ʱ��
	DS3231WriteData(DS3231_ALARM2MINUTE, minute1);
	DS3231WriteData(DS3231_ALARM2SECOND, second1);
	clock[3] = DS3231ReadData(DS3231_ALARM2HOUR); //��ȡ����ʱ��
	clock[4] = DS3231ReadData(DS3231_ALARM2MINUTE);
	clock[5] = DS3231ReadData(DS3231_ALARM2SECOND);
	
	if(clock[0]==CurrentTime.Hour && clock[1]==CurrentTime.Minute && clock[2]==CurrentTime.Second)
	{
		warn(8);
		hcsr= 1;
		led_flash(1, 0, 1);
	//	play = 0; //��ѹ�ı仯����ISD1820�Ĳ���
	//	play = 1;
		flag1=0;
		
		distance();
		S=0;
		hcsr=0;
	  // ���ڳ�ʼ��
	  SCON = 0x50; //8λ���ݣ��ɱ䲨����
	  TMOD = 0x20; //���ö�ʱ��1λ16λ�Զ���װģʽ
	  TL1  = 0xfd; //���ö�ʱ����ֵ
	  TH1  = 0xfd;
	  ET1  = 0; //��ֹ��ʱ��1�ж�
	  TR1  = 1; //������ʱ��1
	  EA   = 1;
		
    delay(8000);
    
	}
		if(clock[3]==CurrentTime.Hour && clock[4]==CurrentTime.Minute&& clock[5]==CurrentTime.Second)
	{
		warn(8);
		hcsr=2;
		led_flash1(1, 0, 1);
	//	play = 0; //��ѹ�ı仯����ISD1820�Ĳ���
	//	play = 1;
		flag1_1=0;
	
		distance_1();
		 S_1=0;
		 hcsr=0;
	  // ���ڳ�ʼ��
	  SCON = 0x50; //8λ���ݣ��ɱ䲨����
	  TMOD = 0x20; //���ö�ʱ��1λ16λ�Զ���װģʽ
	  TL1  = 0xfd; //���ö�ʱ����ֵ
	  TH1  = 0xfd;
	  ET1  = 0; //��ֹ��ʱ��1�ж�
	  TR1  = 1; //������ʱ��1
	  EA   = 1;
		
    delay(8000);
	}

} 

void DS3231_main()
{
	DS3231Init();
	led_off();
//	led_off1();
	if(!BcdByte(DS3231ReadData(DS3231_SECOND)))
	{
		DS3231SetTime(DS3231_HOUR, 11); //����ʱ��Ϊ11:12:41
		DS3231SetTime(DS3231_MINUTE, 12); 
		DS3231SetTime(DS3231_SECOND, 41); 
	}
} 

void time_go()
{ 
	uchar t0, t1, t2, t3, t4, t5;
	DS3231GetTime(&CurrentTime); //��ȡ��ǰʱ�䣬��ʽ���������������ʾ����
	DS3231TimeToStr(&CurrentTime); 
	t0 = CurrentTime.TimeString[0];
	t1 = CurrentTime.TimeString[1];
	t2 = CurrentTime.TimeString[3];
	t3 = CurrentTime.TimeString[4];
	t4 = CurrentTime.TimeString[6];
	t5 = CurrentTime.TimeString[7];
	display(table[t0], table[t1], table[t2], table[t3], table[t4], table[t5]);
	
	hour   = byte_read(0x2000);
	minute = byte_read(0x2010);
	second = byte_read(0x2020);
	AlarmClock(hour, minute, 00,hour1, minute1, 00);
}

