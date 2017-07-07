#ifndef _Led_    //���δ������ͷ�ļ����Ͱ�����
#define _Led_

#define uchar unsigned char    //�궨��һ���޷��ŵ�char����
#define uint unsigned int	   //�궨��һ���޷��ŵ�int����

sbit red   = P2^5;    //�����ɫLED��ΪP2.5 IO��    ��1
sbit green = P2^4;    //������ɫLED��ΪP2.4 IO��
sbit blue  = P2^2;    //������ɫLED��ΪP2.2 IO��

sbit red1   = P1^5;    //�����ɫLED��ΪP1.5 IO��    ��2
sbit green1 = P1^6;    //������ɫLED��ΪP1.6 IO��
sbit blue1  = P1^7;    //������ɫLED��ΪP1.7 IO��*/

void led_off()    //��1ȫ��
{
    red   = 0;
    green = 0;
    blue  = 0;
}

void led_on(uchar r, uchar g, uchar b)    //����1
{	
    red   = r;
    green = g;
    blue  = b;
}

void led_flash(uchar r, uchar g, uchar b)    //��1��˸
{
    led_off();
    delay(50);
    led_on(r, g, b);
    delay(50);
}
void led_off1()    //��2ȫ��
{
    red1   = 0;
    green1 = 0;
    blue1  = 0;
}

void led_on1(uchar r, uchar g, uchar b)    //����2
{	
    red1   = r;
    green1 = g;
    blue1  = b;
} 

void led_flash1(uchar r, uchar g, uchar b)    //��2��˸
{
    led_off1();
    delay(50);
    led_on1(r, g, b);
    delay(50);
} 

#endif