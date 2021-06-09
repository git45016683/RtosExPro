#include "mythread.h"

// 动态创建线程
static rt_thread_t task1Thread;

void TaskCreate(void)
{
//	// 创建信号量
//	uart2_recv_sem = rt_sem_create("uart2_recv_sem", 0, RT_IPC_FLAG_FIFO);
//	if (uart2_recv_sem != RT_NULL)
//	{
//		rt_kprintf("uart2_recv_sem create success");
//	}
//	
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


// 声明信号量-串口2信号量
rt_sem_t uart2_recv_sem;
// 声明线程
static rt_thread_t semExThread;
void SemExTaskCreate(void)
{
	// 创建信号量
	uart2_recv_sem = rt_sem_create("uart2_recv_sem", 0, RT_IPC_FLAG_FIFO);
	if (uart2_recv_sem != RT_NULL)
	{
		rt_kprintf("uart2_recv_sem create success");
	}
	
	// 动态创建线程
	semExThread = rt_thread_create("SemExThread_name",  	// 线程名
																	SemExThread_entery,   // 线程(回调)入口函数
																	RT_NULL,							// 线程参数
																	256,									// 线程堆栈大小
																	3,										// 线程优先级
																	10										// 时间片-多久执行一次
																	);
	if (semExThread != RT_NULL)
	{
		rt_thread_startup(semExThread);
	}
}
MSH_CMD_EXPORT(SemExTaskCreate, create thread to show how use the semaphore)

void SemExThread_entery(void* parameter)
{
	static unsigned int semExTestCount = 0;
	while(1)
	{
		rt_err_t ret = rt_sem_take(uart2_recv_sem, 1000*10/*RT_WAITING_FOREVER*/);
		if (RT_EOK == ret)  // 收到信号量--+1
		{
			printf("\r\ntake a semaphore and ++semExTestCount= %d", ++semExTestCount);
		}
		else if (-RT_ETIMEOUT == ret)  // 超时--输出超时信息
		{
			printf("\r\nmore than 10S did not take a semaphore and semExTestCount is %d", semExTestCount);
		}
		else
		{
			rt_kprintf("\r\nsomething wrong was happened");
		}
	}
}


