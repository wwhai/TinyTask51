#include "reg51.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 flag, r, i; // 定义标志位，取数据的变量r和发送字符的变量i

void UsartInit()
{
    TMOD = 0x20; // 设定定时器1为工作方式2，8位自动重装
    TH1 = 0xF3;  // 装入初值（波特率4800，晶振12M，加倍）
    TL1 = 0xF3;
    PCON = 0x80; // SMOD是TCON的最高位，此处设SMOD为1，1000 0000
    TR1 = 1;     // 启动定时器1
    SCON = 0x50; // 设置串口为工作方式1，允许接收位REN置1，0101 0000
    ES = 1;      // 串口中断允许
    EA = 1;      // 打开总中断
}

void main()
{
    UsartInit();
    while (1)
    {
    }
}
