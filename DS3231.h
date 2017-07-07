#ifndef _DS3231_    //���δ������ͷ�ļ����Ͱ�����
#define _DS3231_

#define uchar unsigned char    //�궨��һ���޷��ŵ�char����
#define uint unsigned int      //�궨��һ���޷��ŵ�int����

sbit DS3231_SDA = P2^0;    //����������
sbit DS3231_SCL = P2^1;    //ʱ��������

bit ACK;    //Ӧ���־λ

typedef struct __DS3231_TIME__    //����ʱ��Ľṹ�����
{
    uchar Second;
    uchar Minute;
    uchar Hour;
    uchar Week;
    uchar Day;
    uchar Month;
    uchar Year;
    uchar DateString[9];    //�����ո�ʽ���ַ���
    uchar TimeString[9];    //ʱ�����ʽ���ַ���
}DS3231_TIME;

#define DS3231_WriteAddress 0xd0    //����д��ַ
#define DS3231_ReadAddress  0xd1    //��������ַ

#define DS3231_SECOND 0x00    //ʱ�ӼĴ�����ز���
#define DS3231_MINUTE 0x01
#define DS3231_HOUR   0x02
#define DS3231_WEEK   0x03
#define DS3231_DAY    0x04
#define DS3231_MONTH  0x05
#define DS3231_YEAR   0x06

#define DS3231_ALARM1SECOND 0x07    //����1
#define DS3231_ALARM1MINUTE 0x08
#define DS3231_ALARM1HOUR   0x09
#define DS3231_ALARM1DAY    0x0A    

#define DS3231_ALARM2MINUTE 0x0B    //����2
#define DS3231_ALARM2HOUR   0x0C
#define DS3231_ALARM2SECOND    0x0D

#define DS3231_CONTROL 0x0E    //���ƼĴ���
#define DS3231_STATUS  0x0F    //״̬�Ĵ���
#define DS3231_XTAL    0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPH   0x11    //�¶ȼĴ������ֽڣ�8λ��
#define DS3231_TEMPL   0x12    //�¶ȼĴ������ֽڣ���2λ��

uchar BcdByte(uchar value)    //BCD��ת��Ϊʮ����
{
    return ((value>>4)*10 + (value&0x0f));
}

uchar ByteBcd(uchar value)    //ʮ����ת��ΪBCD��
{
    return ((value%100)/10<<4 | (value%10));
}

void Delay5()    //�ӳ�5΢�루us��
{;;;;;}

void Start()    //I2C��ʼ�ź�
{
    DS3231_SDA = 1;    //SCL�ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��͵�ƽ�ı仯��ʾ��ʼ�ź�
    Delay5();           
    DS3231_SCL = 1;         
    Delay5();          //��ʼ�źŽ���ʱ�����4.7us
    DS3231_SDA = 0;
    Delay5();          //��ʼ�ź�����ʱ�����4us
    DS3231_SCL = 0;
    Delay5();
}

void Stop()    //I2C��ֹ�ź�
{
    DS3231_SDA = 0;    //SCL�ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ߵ�ƽ�ı仯��ʾ��ֹ�ź�
    Delay5();        
    DS3231_SCL = 1;         
    Delay5();           //��ֹ�źŽ���ʱ�����4us
    DS3231_SDA = 1;
    Delay5();          //��ֹ�ź�����ʱ�����4.7us
}

void SendByte(uchar da)    //����һ�ֽ�����
{
    uchar i;
    for(i = 0; i < 8; i++)    //����8λ����
    {
        if((da<<i)&0x80)      //�жϷ���λ
            DS3231_SDA = 1;   
        else                  
            DS3231_SDA = 0;   
        Delay5();
        DS3231_SCL = 1;       //��ʱ����Ϊ�ߵ�ƽ����֪ͨ��������������λ
        Delay5();             //��֤ʱ�Ӹߵ�ƽ���ڴ���4us
        DS3231_SCL = 0;
    }
    
    Delay5();
    DS3231_SDA = 1;           //8λ���ݷ������ͷ������ߣ���ʼ����Ӧ��λ
    Delay5();
    DS3231_SCL = 1;
    Delay5();
    if(DS3231_SDA == 1)       //�ж��Ƿ��յ�Ӧ���źţ�1-�յ���0-δ�յ�
        ACK = 0;
    else
        ACK = 1;
    DS3231_SCL = 0;           //���ʱ����
    Delay5();
}

uchar ReceiveByte()    //����һ�ֽ�����
{
    uchar i, da = 0;
    DS3231_SDA = 1;        //��������Ϊ���뷽ʽ���ߵ�ƽ��
    for(i = 0; i < 8; i++) 
    {                      
        Delay5();
        DS3231_SCL = 0;    //��ʱ����Ϊ�͵�ƽ��׼����������λ
        Delay5();          //��֤ʱ�ӵ͵�ƽ���ڴ���4.7us
        DS3231_SCL = 1;    //��ʱ����Ϊ�ߵ�ƽ������������������Ч
        Delay5();
        da = da << 1;      //����1λ����λ��0
        if(DS3231_SDA == 1)
            da = da +1;
        Delay5();
    }
    DS3231_SCL = 0;        //���ʱ����
    Delay5();
    return da;
}

void Reponse(bit a)    //Ӧ���ź�
{
    if(a == 0)              //�ж��Ƿ񷢳�Ӧ���ź�
        DS3231_SDA = 0;
    else
        DS3231_SDA = 1;    
    Delay5();
    DS3231_SCL = 1;        //��ʱ����Ϊ�ߵ�ƽ������������������Ч
    Delay5();              //��֤ʱ�Ӹߵ�ƽ���ڴ���4.7us
    DS3231_SCL = 0;
    Delay5();
}

uchar DS3231WriteData(uchar addr, uchar da)    //д�����ݵ�DS3231
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

uchar DS3231Read()    //��ȡ��ǰ��ַ����
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

uchar DS3231ReadData(uchar addr)    //��ȡָ����ַ����
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

void DS3231SetTime(uchar addr, uchar value)    //����ʱ�䣬��ʮ����ת��ΪBCD��
{
    DS3231WriteData(addr, ByteBcd(value));
}

void DS3231GetTime(DS3231_TIME *Time)    //��ȡʱ�䣬��BCD��תΪʮ����
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

/*void DS3231DateToStr(DS3231_TIME *Time)    //�����ո�ʽ������DateString
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

void DS3231TimeToStr(DS3231_TIME *Time)    //ʱ�����ʽ������TimeString
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

void DS3231Init()    //��ʼ��DS3231
{
    DS3231_SDA = 1;
    DS3231_SCL = 1;
}

#endif