#include "mythread.h"

/*************************************线程的创建/使用示例**********************************************/
// 动态创建线程
static rt_thread_t task1Thread;

int TaskCreate(void)
{
	// 动态创建线程
	task1Thread = rt_thread_create("Task1Thread_name",  	// 线程名
																	Task1Thread_entery,   // 线程(回调)入口函数
																	RT_NULL,							// 线程参数
																	256,									// 线程堆栈大小
																	2,										// 线程优先级
																	10										// 时间片-执行多长时间
																	);
	if (task1Thread != RT_NULL)
	{
		rt_thread_startup(task1Thread);
		return 1;
	}
	return 0;
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
//------------------------------------------------------------------------------------
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
																10										// 时间片-执行多长时间
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
/*************************************线程的创建/使用示例**********************************************/

/*************************************信号量的创建/使用示例**********************************************/
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
																	10										// 时间片-执行多长时间
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

//------------------------------------------------------------------------------------
// 静态信号量(init)使用
struct rt_semaphore uart2_recv_sem2;
static struct rt_thread semExThread2;
static char semExThread2Stack[256];
void SemExThreadInit(void)
{
	// 初始化信号量
	rt_err_t ret = rt_sem_init(&uart2_recv_sem2,   // 信号量指针
														 "uart2_recv_sem2",  // 信号量名字
														 0,									 // 初始值
														 RT_IPC_FLAG_FIFO);  // 信号量触发方式-先进先出
	if (ret == RT_EOK)
	{
		rt_kprintf("\r\nuart2_recv_sem2 init success");
	}
	// 初始化线程
	ret = rt_thread_init(&semExThread2,
                        "semExThread2",
                        SemExThread2_entery,
                        RT_NULL,
                        &semExThread2Stack[0],
                        sizeof(semExThread2Stack),
                        3,
                        10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&semExThread2);
	}
}
MSH_CMD_EXPORT(SemExThreadInit, create thread to show how use the semaphore init)

void SemExThread2_entery(void* parameter)
{
	static unsigned int semExTestCount2 = 0;
	while(1)
	{
		rt_err_t ret = rt_sem_take(&uart2_recv_sem2, 1000*10/*RT_WAITING_FOREVER*/);
		if (RT_EOK == ret)  // 收到信号量--+1
		{
			printf("\r\ntake a semaphore and ++semExTestCount2= %d", ++semExTestCount2);
		}
		else if (-RT_ETIMEOUT == ret)  // 超时--输出超时信息
		{
			printf("\r\nmore than 10S did not take a semaphore and semExTestCount2 is %d", semExTestCount2);
		}
		else
		{
			rt_kprintf("\r\nsomething wrong was happened2");
		}
	}
}

/*************************************信号量的创建/使用示例**********************************************/


