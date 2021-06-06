#include "mythread.h"

// 动态创建线程
static rt_thread_t task1Thread;

void TaskInit(void)
{
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

void Task1Thread_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	while(1)
	{
		printf("\r\ntask1TestCount: %d", task1TestCount++);
		rt_thread_mdelay(1000);
	}
}






//// 静态创建线程
//static rt_thread_t task2Thread;
//static char task2Stack[1024];

//	rt_thread_init(task2Thread,						// 线程指针
//								 "Task2Thread_name",  	// 线程名
//									Task2Thread_entery,   // 线程(回调)入口函数
//									RT_NULL,							// 线程参数
//									&task2Stack[0],				// 线程堆栈起始地址
//									sizeof(task2Stack),		// 线程堆栈大小
//									2,										// 线程优先级
//									10										// 时间片-多久执行一次
//									);
//	if (task2Thread != RT_NULL)
//	{
//		rt_thread_startup(task2Thread);
//	}


//void Task2Thread_entery(void* parameter)
//{
//	static unsigned int task2TestCount = 0;
//	while(1)
//	{
//		printf("\r\ntask2TestCount: %d", task2TestCount++);
//		rt_thread_mdelay(1000*5);
//	}
//}
