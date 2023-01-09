#ifndef _TINY_TASK_H_
#define _TINY_TASK_H_

typedef unsigned char uint8_t;

/// @brief 任务状态
typedef enum TaskState {
    RUNNING = 0, // 正在运行
    READY,       // 准备好了
    BLOCK,       // 被挂起来(sleep)
    STOP         // 已经结束
} TaskState;
/// @brief 任务类型枚举
typedef enum TaskType {
    IO = 0,   // IO 任务, 例如串口, 当任务是IO类型的时候，会被加入一个类似Linux Select的表里面
    CALCULATE // 计算任务，普通任务
} TaskType;
/// @brief 任务控制参数
typedef struct TaskControl {
    volatile uint8_t valid : 1;    // 是否是有效任务
    volatile uint8_t switched : 1; // 是否已经被保存了上下文
    TaskType task_type : 2;        // 任务的类型, 0:IO; 1: Calculate
    volatile uint8_t priority : 4; // 任务的优先级 0-16, 越小越优先
    TaskState state;               // 任务的当前状态
} TaskControl;

/// @brief 任务函数
typedef void (*TaskFunc)(void *args);
/// @brief 任务定义
typedef struct {
    uint8_t id;                        // ID => [0,16]
    volatile TaskControl task_control; // 任务控制参数
    volatile uint8_t time_slice;       // 分配给任务的时间片，默认一个任务时间片为100毫秒
    volatile unsigned long sleep_time; // 假设给sleep了，这里表示sleep的时间
    TaskFunc func;                     // 任务执行函数
    uint8_t local_mailbox[16];         // 本地邮箱, 本质上会被设计成一个消息队列, 用来做进程间通信
} Task;
/// @brief RTOS实例
typedef struct TinyTask {
    Task tasks[16]; // 任务列表
} TinyTask;

/// @brief 初始化
/// @param
TinyTask TT_init(void);

/// @brief  停止
/// @param tt
void TT_stop(TinyTask tt);

/// @brief 执行任务
/// @param task
void TT_go(TinyTask tt, Task task);

/// @brief 取消一个任务
/// @param tt
/// @param task_id
void TT_cancel(TinyTask tt, unsigned char task_id);

/// @brief 主动让出CPU
/// @param tt
void TT_yield(TinyTask tt);

/// @brief 任务表
/// @param tt
Task *TT_task_list(TinyTask tt);

#endif