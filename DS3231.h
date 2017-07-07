#ifndef _DS3231_    //如果未包含该头文件，就包含它
#define _DS3231_

#define uchar unsigned char    //宏定义一个无符号的char类型
#define uint unsigned int      //宏定义一个无符号的int类型

sbit DS3231_SDA = P2^0;    //数据线引脚
sbit DS3231_SCL = P2^1;    //时钟线引脚

bit ACK;    //应答标志位

typedef struct __DS3231_TIME__    //定义时间的结构体变量
{
    uchar Second;
    uchar Minute;
    uchar Hour;
    uchar Week;
    uchar Day;
    uchar Month;
    uchar Year;
    uchar DateString[9];    //年月日格式化字符串
    uchar TimeString[9];    //时分秒格式化字符串
}DS3231_TIME;

#define DS3231_WriteAddress 0xd0    //器件写地址
#define DS3231_ReadAddress  0xd1    //器件读地址

#define DS3231_SECOND 0x00    //时钟寄存器相关参数
#define DS3231_MINUTE 0x01
#define DS3231_HOUR   0x02
#define DS3231_WEEK   0x03
#define DS3231_DAY    0x04
#define DS3231_MONTH  0x05
#define DS3231_YEAR   0x06

#define DS3231_ALARM1SECOND 0x07    //闹钟1
#define DS3231_ALARM1MINUTE 0x08
#define DS3231_ALARM1HOUR   0x09
#define DS3231_ALARM1DAY    0x0A    

#define DS3231_ALARM2MINUTE 0x0B    //闹钟2
#define DS3231_ALARM2HOUR   0x0C
#define DS3231_ALARM2SECOND    0x0D

#define DS3231_CONTROL 0x0E    //控制寄存器
#define DS3231_STATUS  0x0F    //状态寄存器
#define DS3231_XTAL    0x10    //晶体老化寄存器
#define DS3231_TEMPH   0x11    //温度寄存器高字节（8位）
#define DS3231_TEMPL   0x12    //温度寄存器高字节（高2位）

uchar BcdByte(uchar value)    //BCD码转换为十进制
{
    return ((value>>4)*10 + (value&0x0f));
}

uchar ByteBcd(uchar value)    //十进制转换为BCD码
{
    return ((value%100)/10<<4 | (value%10));
}

void Delay5()    //延迟5微秒（us）
{;;;;;}

void Start()    //I2C起始信号
{
    DS3231_SDA = 1;    //SCL在高电平期间，SDA由高电平向低电平的变化表示起始信号
    Delay5();           
    DS3231_SCL = 1;         
    Delay5();          //起始信号建立时间大于4.7us
    DS3231_SDA = 0;
    Delay5();          //起始信号锁定时间大于4us
    DS3231_SCL = 0;
    Delay5();
}

void Stop()    //I2C终止信号
{
    DS3231_SDA = 0;    //SCL在高电平期间，SDA由低电平向高电平的变化表示终止信号
    Delay5();        
    DS3231_SCL = 1;         
    Delay5();           //终止信号建立时间大于4us
    DS3231_SDA = 1;
    Delay5();          //终止信号锁定时间大于4.7us
}

void SendByte(uchar da)    //发送一字节数据
{
    uchar i;
    for(i = 0; i < 8; i++)    //发送8位数据
    {
        if((da<<i)&0x80)      //判断发送位
            DS3231_SDA = 1;   
        else                  
            DS3231_SDA = 0;   
        Delay5();
        DS3231_SCL = 1;       //置时钟线为高电平，即通知被控器接收数据位
        Delay5();             //保证时钟高电平周期大于4us
        DS3231_SCL = 0;
    }
    
    Delay5();
    DS3231_SDA = 1;           //8位数据发送完释放数据线，开始接受应答位
    Delay5();
    DS3231_SCL = 1;
    Delay5();
    if(DS3231_SDA == 1)       //判断是否收到应答信号，1-收到，0-未收到
        ACK = 0;
    else
        ACK = 1;
    DS3231_SCL = 0;           //清空时钟线
    Delay5();
}

uchar ReceiveByte()    //接受一字节数据
{
    uchar i, da = 0;
    DS3231_SDA = 1;        //置数据线为输入方式（高电平）
    for(i = 0; i < 8; i++) 
    {                      
        Delay5();
        DS3231_SCL = 0;    //置时钟线为低电平，准备接收数据位
        Delay5();          //保证时钟低电平周期大于4.7us
        DS3231_SCL = 1;    //置时钟线为高电平，即数据线上数据有效
        Delay5();
        da = da << 1;      //左移1位，低位补0
        if(DS3231_SDA == 1)
            da = da +1;
        Delay5();
    }
    DS3231_SCL = 0;        //清空时钟线
    Delay5();
    return da;
}

void Reponse(bit a)    //应答信号
{
    if(a == 0)              //判断是否发出应答信号
        DS3231_SDA = 0;
    else
        DS3231_SDA = 1;    
    Delay5();
    DS3231_SCL = 1;        //置时钟线为高电平，即数据线上数据有效
    Delay5();              //保证时钟高电平周期大于4.7us
    DS3231_SCL = 0;
    Delay5();
}

uchar DS3231WriteData(uchar addr, uchar da)    //写入数据到DS3231
{
    Start();
    SendByte(DS3231_WriteAddress);
    if(ACK == 0)
        return 0;
    SendByte(addr);
    
    if(ACK == 0)
        return 0;
    SendByte(da);
    
    if(ACK == 0)
        return 0;
    Stop();
    Delay5();
    return 1;  
}

uchar DS3231Read()    //读取当前地址数据
{
    uchar da;
    Start();
    SendByte(DS3231_ReadAddress);
    if(ACK == 0)
        return 0;
    da = ReceiveByte();
    Reponse(1);
    Stop();
    return da;
}

uchar DS3231ReadData(uchar addr)    //读取指定地址数据
{
    Start();
    SendByte(DS3231_WriteAddress);
    if(ACK == 0)
        return 0;
    SendByte(addr);
    
    if(ACK == 0)
        return 0;
    return DS3231Read();
}

void DS3231SetTime(uchar addr, uchar value)    //设置时间，将十进制转换为BCD码
{
    DS3231WriteData(addr, ByteBcd(value));
}

void DS3231GetTime(DS3231_TIME *Time)    //获取时间，将BCD码转为十进制
{
    uchar ReadValue;
    ReadValue = DS3231ReadData(DS3231_SECOND);
    Time->Second = BcdByte(ReadValue);   
    ReadValue = DS3231ReadData(DS3231_MINUTE);
    Time->Minute = BcdByte(ReadValue);    
    ReadValue = DS3231ReadData(DS3231_HOUR);
    Time->Hour = BcdByte(ReadValue);    
    ReadValue = DS3231ReadData(DS3231_DAY);
    Time->Day = BcdByte(ReadValue);     
    ReadValue = DS3231ReadData(DS3231_WEEK);
    Time->Week = BcdByte(ReadValue);   
    ReadValue = DS3231ReadData(DS3231_MONTH);
    Time->Month = BcdByte(ReadValue);   
    ReadValue = DS3231ReadData(DS3231_YEAR);
    Time->Year = BcdByte(ReadValue); 
}

/*void DS3231DateToStr(DS3231_TIME *Time)    //年月日格式化存入DateString
{
    Time->DateString[0] = Time->Year/10;
    Time->DateString[1] = Time->Year%10;
    Time->DateString[2] = '-';
    Time->DateString[3] = Time->Month/10;
    Time->DateString[4] = Time->Month%10;
    Time->DateString[5] = '-';
    Time->DateString[6] = Time->Day/10;
    Time->DateString[7] = Time->Day%10;
    Time->DateString[8] = '\0';
}*/

void DS3231TimeToStr(DS3231_TIME *Time)    //时分秒格式化存入TimeString
{
    Time->TimeString[0] = Time->Hour/10;
    Time->TimeString[1] = Time->Hour%10;
    Time->TimeString[2] = ':';
    Time->TimeString[3] = Time->Minute/10;
    Time->TimeString[4] = Time->Minute%10;
    Time->TimeString[5] = ':';
    Time->TimeString[6] = Time->Second/10;
    Time->TimeString[7] = Time->Second%10;
    Time->TimeString[8] = '\0';
}

void DS3231Init()    //初始化DS3231
{
    DS3231_SDA = 1;
    DS3231_SCL = 1;
}

#endif