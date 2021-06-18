#include "mycriticalsuspend.h"

// 1-taskENTER_CRITICAL--基本临界区
// 2-vTaskSuspendAll--关闭系统任务调度器
#define TYPE		2  // 1-_CRITICAL  2-vTaskSuspendAll  
#define delayTime 0

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
void TaskCreateForCritical(void)
{
	printf("\r\nTaskCreateForSemaphore");
	BaseType_t ret = xTaskCreate(CriticalTask1_entery,						// 任务入口函数
															"CriticalTask1_entery",						// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															1,																// 任务优先级
															&taskHandle);											// 任务句柄
	
	ret = xTaskCreate(CriticalTask3_entery,												// 任务入口函数
															"CriticalTask3_entery",						// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															1,																// 任务优先级
															&taskHandle);											// 任务句柄
}

static unsigned int TestCount = 0;
void CriticalTask1_entery(void* parameter)
{
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#endif
		for(int i = 0; i < 10; i++)
		{
			printf("\r\n1.TestCount: %d", TestCount++);
			vTaskDelay(0);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#endif
	}
}
void CriticalTask3_entery(void* parameter)
{
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#endif
		for(int i = 0; i < 10; i++)
		{
			printf("\r\n3.TestCount: %d", TestCount++);
			vTaskDelay(delayTime);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#endif
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStaticForCritical(void)
{
	printf("\r\nTaskCreateStaticForSemaphore");
	taskStaticHandle = xTaskCreateStatic(CriticalTask2_entery,		// 任务入口函数
																			"CriticalTask2_entery",		// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			5,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}

void CriticalTask2_entery(void* parameter)
{
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#endif
		for(int i = 0; i < 10; i++)
		{
			printf("\r\n2.TestCount: %d", TestCount++);
			vTaskDelay(delayTime);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#endif
	}
}






