#ifndef _TINY_TASK_H_
#define _TINY_TASK_H_

#define MAX_TASKS 16
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
    volatile uint8_t priority : 4; // 任务的优先级 0-16, 越小越优先,0 表示不需要优先级
    TaskState state;               // 任务的当前状态
} TaskControl;

/// @brief 任务函数
typedef void (*TaskFunc)(void *args);
/// @brief 任务定义
typedef struct {
    uint8_t id;                        // ID => [0,16]
    unsigned short context_sp;         // 保存的上下文SP纸箱的 21个寄存器的首地址
    volatile TaskControl task_control; // 任务控制参数
    volatile char time_slice;          // 分配给任务的时间片，默认一个任务时间片为100毫秒
    volatile long sleep_time;          // 假设给sleep了，这里表示sleep的时间
    TaskFunc func;                     // 任务执行函数
    void *args;                        // 任务执行函数的参数
    char local_mailbox[2];             // 本地邮箱, 本质上会被设计成一个消息队列, 用来做进程间通信, -1 表示无消息
} Task;
/// @brief RTOS实例
typedef struct TinyTask {
    Task tasks[2];           // 任务列表
    uint8_t current_task_id; // 当前执行的任务ID
} TinyTask;

/// @brief 初始化
/// @param
void TT_init(TinyTask *tt);
/// @brief 启动RTOS
/// @param tt
void TT_start(TinyTask *tt);
/// @brief  停止
/// @param tt
void TT_stop(TinyTask *tt);

/// @brief 执行任务
/// @param task
void TT_go(TinyTask *tt, Task task);

/// @brief 取消一个任务
/// @param tt
/// @param task_id
void TT_cancel(TinyTask *tt, unsigned char task_id);

/// @brief 主动让出CPU
/// @param tt
void TT_yield(TinyTask *tt);

/// @brief 任务表
/// @param tt
void TT_task_list(TinyTask *tt);

#endif