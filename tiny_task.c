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
/// @brief 初始化
/// @param
void TT_init(TinyTask *tt)
{
    T0Init();
    tt->current_task_id = 0;
}
/// @brief 启动OS
/// @param tt
void TT_start(TinyTask *tt)
{
    // Task0 表示系统进程
    unsigned char i;
    while (1) {
        for (i = 0; i < MAX_TASKS; i++) {
            if (tt->tasks[i].task_control.valid) {
                if (tt->tasks[i].task_control.state == READY) {
                    tt->tasks[i].task_control.state == RUNNING;
                    tt->tasks[i].func(tt->tasks[i].args);
                    tt->tasks[i].task_control.state == STOP;
                }
            }
        }
    }
}
/// @brief  停止
/// @param tt
void TT_stop(TinyTask *tt)
{
    unsigned char i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (tt->tasks[i].task_control.valid) {
            tt->tasks[i].task_control.valid = 0;
        }
        if (tt->tasks[i].time_slice > 0) {
            tt->tasks[i].time_slice = -1;
        }
    }
}

/// @brief 执行任务
/// @param task
void TT_go(TinyTask *tt, Task task)
{
    unsigned char i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (!tt->tasks[i].task_control.valid) {
            tt->tasks[i] = task;
        }
    }
}

/// @brief 取消一个任务
/// @param tt
/// @param task_id
void TT_cancel(TinyTask *tt, unsigned char task_id)
{
    if (tt->tasks[task_id].task_control.valid) {
        tt->tasks[task_id].task_control.valid = 0;
    }
    if (tt->tasks[task_id].time_slice > 0) {
        tt->tasks[task_id].time_slice = -1;
    }
    if (tt->tasks[task_id].sleep_time > 0) {
        tt->tasks[task_id].sleep_time = -1;
    }
}

/// @brief 主动让出CPU
/// @param tt
void TT_yield(TinyTask *tt)
{
    tt->tasks[tt->current_task_id].context_sp = SP; // 保存上下文
    if (tt->current_task_id + 1 > MAX_TASKS) {
        tt->current_task_id = 0; // 说明到底了, 回到第一个任务
    }
    // 此处应该处理优先级, 不过目前暂时先实现最简单的切换效果
    // --> 优先级 MIN = 排序任务表()
    // --> yield-to-min(SP = tt->tasks[优先级最小的].contet_sp)
    SP = tt->tasks[tt->current_task_id + 1].context_sp;
}

/// @brief 任务表
/// @param tt
void TT_task_list(TinyTask *tt)
{
    // printf("--------------------------")
    // printf("--------------------------")
}

/// @brief T0中断函数

void T0_IRQ_Handler() interrupt 1
{
    TL0 = 0x00; // 设置定时初始值: 5ms
    TH0 = 0x28; // 设置定时初始值: 5ms
}
///
void t0f(void *args)
{
    while (1)
        ;
}
///
void t1f(void *args)
{
}
void t2f(void *args)
{
}
/// @brief Main
TinyTask tt;
void main()
{
    TT_init(&tt);
    // task1
    Task t0;
    t0.id                     = 0;
    t0.context_sp             = 0;
    t0.time_slice             = 100;
    t0.sleep_time             = 0;
    t0.task_control.state     = READY;
    t0.task_control.valid     = 1;
    t0.task_control.task_type = CALCULATE;
    t0.task_control.priority  = 0;
    t0.func                   = t0f;
    TT_go(&tt, t0);
    TT_start(&tt);
}
