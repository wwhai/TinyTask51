#include <reg51.h>
#include "tiny_task.h"
/// @brief 初始化T0，而T0就是我们的TinyTask调度要用到的定时器
/// @param
void T0Init(void)
{
    EA   = 1;
    TMOD = 0x01;
    TL0  = 0x00; // 设置定时初始值: 5ms
    TH0  = 0x28; // 设置定时初始值: 5ms
    TR0  = 1;
    ET0  = 1;
}
/// @brief T0中断函数

void T0_IRQ_Handler() interrupt 1
{
    TL0 = 0x00; // 设置定时初始值: 5ms
    TH0 = 0x28; // 设置定时初始值: 5ms
}

/// @brief Main
void main()
{
    T0Init();
}
