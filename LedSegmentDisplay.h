#ifndef _LedSegmentDisplayDisplay_    //���δ������ͷ�ļ����Ͱ�����
#define _LedSegmentDisplayDisplay_

#define uchar unsigned char    //�궨��һ���޷��ŵ�char����
#define uint unsigned int      //�궨��һ���޷��ŵ�int����
    
 void LED_OUT(uchar X);
    
void delay(uint z)    //������ʱ����,z����
{
	uint x, y;
	for(x = z; x > 0; x--)
		for(y = 112; y > 0; y--);
}
 
unsigned char code LED_0F[];		// LED��ģ��

sbit DIO = P1^0;				//������������
sbit RCLK1  = P1^1;				//ʱ�������źš�����������Ч
sbit SCLK = P1^2;				//�����źš���������������Ч

//-----------------------------------------------------------------------------
// ȫ�ֱ���
	
	unsigned char code *led_table;          // ���ָ��
	uchar i; 

//*****************************************************************************
// ������
//
 

void display (uchar sw, uchar wan, uchar qian, uchar bai, uchar shi, uchar ge)
{
   

	//��ʾ��1λ


	LED_OUT(ge);			
	LED_OUT(0x01);		

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��2λ
	

	LED_OUT(shi);		
	LED_OUT(0x02);		

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��3λ
	led_table = LED_0F + 16;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��4λ
 
	LED_OUT(bai);			
	LED_OUT(0x08);		

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��5λ
 

	LED_OUT(qian);			
	LED_OUT(0x10);	

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��6λ
	led_table = LED_0F +16;
	i = *led_table;

	LED_OUT(i);			
 	LED_OUT(0x20);	

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��7λ
 

	LED_OUT(wan);			
	LED_OUT(0x40);	

	RCLK1 = 0;
	RCLK1 = 1;
	//��ʾ��8λ
 
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