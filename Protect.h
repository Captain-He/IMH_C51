#ifndef _Protect_ //如果未包含该头文件，就包含它
#define _Protect_

#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
//定义ISP的操作命令
#define RdCommand 0x01 //字节读
#define PrgCommand 0x02  //字节写
#define EraseCommand 0x03  //扇区擦除

#define Error 1
#define Ok 0

#define WaitTime 0x01 //定义CPU的等待时间
//寄存器申明	，1110 0***工作寄存器0
sfr ISP_DATA=0xe2;  //Flash数据寄存器
sfr ISP_ADDRH=0xe3; //Flash高字节地址寄存器
sfr ISP_ADDRL=0xe4; //Flash低字节地址寄存器
sfr ISP_CMD=0xe5; 	//Flash命令模式寄存器
sfr ISP_TRIG=0xe6; 	//Flash命令触发寄存器
sfr ISP_CONTR=0xe7; //ISP/IAP 控制寄存器
//sbit dula=P2^6; //申明U1锁存器的锁存端
//sbit wela=P2^7; //申明U2锁存器的锁存端
/*unsigned long num;
uchar j,k;
uchar a0,b0,c0,d0,e0,f0,g0,h0;
sbit S2=P3^0;
uchar code table[]={
                    0x3f,
                    0x06,
                    0x5b,
                    0x4f,
                    0x66,
                    0x6d,
                    0x7d,
                    0x07,
                    0x7f,
                    0x6f,
                    0x77,
                    0x7c,
                    0x39,
                    0x5e,
                    0x79,
                    0x71
                    };

void delay(uint x)
{
    uint i,j;

    for(i=x;i>0;i--)//i=x即延时约x毫秒
        for(j=110;j>0;j--);
}
void display(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f,uchar g,uchar h)
{
   dula=0;
   P0=table[a];
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);

   P0=table[b];
   dula=1;
   dula=0;

   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[c];
   dula=1;
   dula=0;

   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);

   P0=table[d];
   dula=1;
   dula=0;

   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);

   P0=table[e];
   dula=1;
   dula=0;

   P0=0xef;
   wela=1;
   wela=0;
   delay(5);

   P0=table[f];
   dula=1;
   dula=0;

   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);

   P0=table[g];
   dula=1;
   dula=0;

   P0=0xbf;
   wela=1;
   wela=0;
   delay(5);

   P0=table[h];
   dula=1;
   dula=0;

   P0=0x7f;
   wela=1;
   wela=0;
   delay(5);
}*/
/*================打开ISP,IAP功能=================*/
void ISP_IAP_enable(void)
{
    EA = 0;//关中断
    ISP_CONTR = ISP_CONTR & 0x18;//1110 0111&0001 1000=0000 0000
    ISP_CONTR = ISP_CONTR | WaitTime;//写入硬件延时1110 0111
    ISP_CONTR = ISP_CONTR | 0x80;//ISPEN=1 ，1110 0111
}
/*===============关闭ISP,IAP功能==================*/
void ISP_IAP_disable(void)
{
    ISP_CONTR = ISP_CONTR & 0x7f; //ISPEN = 0
    ISP_TRIG  = 0x00;
    EA = 1; //开中断
}
/*================公用的触发代码====================*/
void ISPgoon(void)
{
    ISP_IAP_enable();//打开ISP,IAP功能
    ISP_TRIG = 0x46;//触发ISP_IAP命令字节1
    ISP_TRIG = 0xb9;//触发ISP_IAP命令字节2
    _nop_();
}
/*====================字节读========================*/
unsigned char byte_read(uint byte_addr)
{
    ISP_ADDRH = (uchar)(byte_addr >> 8);//地址赋值
    ISP_ADDRL = (uchar)(byte_addr & 0x00ff);
    ISP_CMD   = ISP_CMD & 0xf8;//清除低3位
    ISP_CMD   = ISP_CMD | RdCommand;//写入读命令
    ISPgoon();//触发执行
    ISP_IAP_disable();//关闭ISP,IAP功能
    return (ISP_DATA);//返回读到的数据
}
/*==================扇区擦除========================*/
void SectorErase(uint sector_addr)
{
    uint iSectorAddr;
    iSectorAddr = (sector_addr & 0xfe00);//取扇区地址
    ISP_ADDRH = (uchar)(iSectorAddr >> 8);
    ISP_ADDRL = 0x00;
	ISP_CMD = ISP_CMD & 0xf8;//清空低3位
    ISP_CMD = ISP_CMD | EraseCommand;//擦除命令3
    ISPgoon();//触发执行
    ISP_IAP_disable();//关闭ISP,IAP功能
}
/*====================字节写========================*/
void byte_write(uint byte_addr, uchar original_data)
{
    ISP_ADDRH = (uchar)(byte_addr >> 8);//取地址
    ISP_ADDRL = (uchar)(byte_addr & 0x00ff);
    ISP_CMD   = ISP_CMD & 0xf8;//清低3位
    ISP_CMD   = ISP_CMD | PrgCommand;//写命令
    ISP_DATA  = original_data;//写入数据准备
    ISPgoon();//触发执行
    ISP_IAP_disable();//关闭IAP功能
}
/*void keyscan()
{
	if(S2==0)
	{
		while(S2==0);
		num++;
		SectorErase(0x2000);//擦除扇区
      	byte_write(0x2000,num);//重新写入数据
	}
}
void main()
{
    num=byte_read(0x2000);//程序开始时读取EEPROM扇区一中数据
    while(1)
    {
		keyscan();
		if(num<10)
		{
			a0=num;b0=16;c0=16;d0=16;e0=16;f0=16;g0=16;h0=16;
		}
		else
		{
			if(num<100)
			{a0=num/10;b0=num%10;c0=16;d0=16;e0=16;f0=16;g0=16;h0=16;}
			else
			{
				if(num<1000)
				{a0=num/100;b0=num%100/10;c0=num%10;d0=16;e0=16;f0=16;g0=16;h0=16;}
				else
				{
					if(num<10000)
					{a0=num/1000;b0=num%1000/100;c0=num%100/10;d0=num%10;e0=16;f0=16;g0=16;h0=16;}
					else
					{
						if(num<100000)
						{a0=num/10000;b0=num%10000/1000;c0=num%1000/100;d0=num%100/10;e0=num%10;f0=16;g0=16;h0=16;}
						else
						{
							if(num<1000000)
							{a0=num/100000;b0=num%100000/10000;c0=num%10000/1000;d0=num%1000/100;e0=num%100/10;f0=num%10;g0=16;h0=16;}
							else
							{
						   		if(num<10000000)
						   	 	{a0=num/1000000;b0=num%1000000/100000;c0=num%100000/10000;d0=num%10000/1000;e0=num%1000/100;f0=num%100/10;g0=num%10;h0=16;}
								else
								{
							 	   if(num<100000000)
					        		{a0=num/10000000;b0=num%10000000/1000000;c0=num%1000000/100000;d0=num%100000/10000;e0=num%10000/1000;f0=num%1000/100;g0=num%100/10;h0=num%10;}
								}
							}
						}
					}
				}
			}
		}
        display(a0,b0,c0,d0,e0,f0,g0,h0);
    }
}*/

#endif
