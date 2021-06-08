#include "mythread.h"

// 动态创建线程
static rt_thread_t task1Thread;

void TaskCreate(void)
{
	// 动态创建线程
	task1Thread = rt_thread_create("Task1Thread_name",  	// 线程名
																	Task1Thread_entery,   // 线程(回调)入口函数
																	RT_NULL,							// 线程参数
																	256,									// 线程堆栈大小
																	2,										// 线程优先级
																	10										// 时间片-多久执行一次
																	);
	if (task1Thread != RT_NULL)
	{
		rt_thread_startup(task1Thread);
	}
}
INIT_APP_EXPORT(TaskCreate);

void Task1Thread_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	while(1)
	{
		printf("\r\ntask1TestCount: %d", task1TestCount++);
		rt_thread_mdelay(1000*10);
	}
}


// 静态创建线程
ALIGN(RT_ALIGN_SIZE)  // 变量分配4字节对齐
static struct rt_thread task2Thread;  // 线程控制块
static char task2Stack[256];  // 线程堆栈定义

void TaskInit(void)
{	
	// 静态创建线程
	rt_err_t ret = rt_thread_init(&task2Thread,					// 线程控制块指针
															 "Task2Thread_name",  	// 线程名
																Task2Thread_entery,   // 线程(回调)入口函数
																RT_NULL,							// 线程参数
																&task2Stack[0],				// 线程堆栈起始地址
																sizeof(task2Stack),		// 线程堆栈大小
																2,										// 线程优先级
																10										// 时间片-多久执行一次
																);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&task2Thread);
	}
}
MSH_CMD_EXPORT(TaskInit, init task2Thread by finsh and output debug information 8s)

void Task2Thread_entery(void* parameter)
{
	static unsigned int task2TestCount = 0;
	while(1)
	{
		printf("\r\ntask2TestCount: %d", task2TestCount++);
		rt_thread_mdelay(1000*8);
	}
}
