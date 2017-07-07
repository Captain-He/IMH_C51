#ifndef _Led_    //如果未包含该头文件，就包含它
#define _Led_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int	   //宏定义一个无符号的int类型

sbit red   = P2^5;    //定义红色LED，为P2.5 IO口    灯1
sbit green = P2^4;    //定义绿色LED，为P2.4 IO口
sbit blue  = P2^2;    //定义蓝色LED，为P2.2 IO口

sbit red1   = P1^5;    //定义红色LED，为P1.5 IO口    灯2
sbit green1 = P1^6;    //定义绿色LED，为P1.6 IO口
sbit blue1  = P1^7;    //定义蓝色LED，为P1.7 IO口*/

void led_off()    //灯1全灭
{
    red   = 0;
    green = 0;
    blue  = 0;
}

void led_on(uchar r, uchar g, uchar b)    //亮灯1
{	
    red   = r;
    green = g;
    blue  = b;
}

void led_flash(uchar r, uchar g, uchar b)    //灯1闪烁
{
    led_off();
    delay(50);
    led_on(r, g, b);
    delay(50);
}
void led_off1()    //灯2全灭
{
    red1   = 0;
    green1 = 0;
    blue1  = 0;
}

void led_on1(uchar r, uchar g, uchar b)    //亮灯2
{	
    red1   = r;
    green1 = g;
    blue1  = b;
} 

void led_flash1(uchar r, uchar g, uchar b)    //灯2闪烁
{
    led_off1();
    delay(50);
    led_on1(r, g, b);
    delay(50);
} 

#endif