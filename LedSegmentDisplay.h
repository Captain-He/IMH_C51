#ifndef _LedSegmentDisplayDisplay_    //如果未包含该头文件，就包含它
#define _LedSegmentDisplayDisplay_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int      //宏定义一个无符号的int类型
    
 void LED_OUT(uchar X);
    
void delay(uint z)    //进行延时处理,z毫秒
{
	uint x, y;
	for(x = z; x > 0; x--)
		for(y = 112; y > 0; y--);
}
 
unsigned char code LED_0F[];		// LED字模表

sbit DIO = P1^0;				//串行数据输入
sbit RCLK1  = P1^1;				//时钟脉冲信号——上升沿有效
sbit SCLK = P1^2;				//打入信号————上升沿有效

//-----------------------------------------------------------------------------
// 全局变量
	
	unsigned char code *led_table;          // 查表指针
	uchar i; 

//*****************************************************************************
// 主程序
//
 

void display (uchar sw, uchar wan, uchar qian, uchar bai, uchar shi, uchar ge)
{
   

	//显示第1位


	LED_OUT(ge);			
	LED_OUT(0x01);		

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第2位
	

	LED_OUT(shi);		
	LED_OUT(0x02);		

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第3位
	led_table = LED_0F + 16;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第4位
 
	LED_OUT(bai);			
	LED_OUT(0x08);		

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第5位
 

	LED_OUT(qian);			
	LED_OUT(0x10);	

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第6位
	led_table = LED_0F +16;
	i = *led_table;

	LED_OUT(i);			
 	LED_OUT(0x20);	

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第7位
 

	LED_OUT(wan);			
	LED_OUT(0x40);	

	RCLK1 = 0;
	RCLK1 = 1;
	//显示第8位
 
	LED_OUT(sw);		
	LED_OUT(0x80);	

	RCLK1 = 0;
	RCLK1 = 1;
	

}

void LED_OUT(uchar X)
{
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO=1; else DIO=0;
		X<<=1;
		SCLK = 0;
		SCLK = 1;
	}
}

unsigned char code LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};



 

#endif