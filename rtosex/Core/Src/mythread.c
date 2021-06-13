#include "mythread.h"

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄

void TaskCreate(void)
{
	printf("\r\nTaskCreate");
	BaseType_t ret = xTaskCreate(Task1Thread_entery,							// 任务入口函数
															"Task1Thread_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&taskHandle);											// 任务句柄
}

void Task1Thread_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	while(1)
	{
		printf("\r\n--task1TestCount: %d", task1TestCount++);
		vTaskDelay(configTICK_RATE_HZ*3);
	}
}

//// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStatic(void)
{
	printf("\r\nTaskCreateStatic");
	taskStaticHandle = xTaskCreateStatic(Task2Thread_entery,			// 任务入口函数
																			"Task2Thread_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}


void Task2Thread_entery(void* parameter)
{
	static unsigned int task2TestCount = 0;
	while(1)
	{
		printf("\r\ntask2TestCount: %d", task2TestCount++);
		vTaskDelay(configTICK_RATE_HZ);
	}
}
