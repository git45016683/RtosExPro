#include "mycriticalsuspend.h"

// 1-taskENTER_CRITICAL--基本临界区
// 2-vTaskSuspendAll--关闭系统任务调度器
// 3-Mutex--互斥量
#define TYPE		3  // 1-_CRITICAL  2-vTaskSuspendAll  3-Mutex
#define delayTime 1000
#define TIMEOUT  10

// 互斥量声明
static xSemaphoreHandle semaphrMutexHandle = NULL;

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
void TaskCreateForCritical(void)
{
//	createMutex();
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
	BaseType_t ret = NULL;
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#elif TYPE == 3
		ret = xSemaphoreTake(semaphrMutexHandle, TIMEOUT);
		if (ret == pdTRUE)
		{
		#endif
		for(int i1 = 0; i1 < 10; i1++)
		{
			printf("\r\n1.TestCount: %d ->%d", TestCount++, i1);
			vTaskDelay(delayTime);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#elif TYPE == 3
		ret = xSemaphoreGive(semaphrMutexHandle);
		taskYIELD();
		}
		#endif
	}
}
void CriticalTask3_entery(void* parameter)
{
	BaseType_t ret = NULL;
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#elif TYPE == 3
		ret = xSemaphoreTake(semaphrMutexHandle, TIMEOUT);
		if (ret == pdTRUE)
		{
		#endif
		for(int i3 = 0; i3 < 10; i3++)
		{
			printf("\r\n3.TestCount: %d ->%d", TestCount++, i3);
			vTaskDelay(delayTime);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#elif TYPE == 3
		ret = xSemaphoreGive(semaphrMutexHandle);
		taskYIELD();
		}
		#endif
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStaticForCritical(void)
{
	printf("\r\nTaskCreateStaticForSemaphore mutex:%d",*(uint32_t*)semaphrMutexHandle);
	taskStaticHandle = xTaskCreateStatic(CriticalTask2_entery,		// 任务入口函数
																			"CriticalTask2_entery",		// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			2,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}

void CriticalTask2_entery(void* parameter)
{
	BaseType_t ret = NULL;
	while(1)
	{
		#if TYPE == 1
		taskENTER_CRITICAL();
		#elif TYPE == 2
		vTaskSuspendAll();
		#elif TYPE == 3
		ret = xSemaphoreTake(semaphrMutexHandle, TIMEOUT);
		if (ret == pdTRUE)
		{
		#endif
		for(int i2 = 0; i2 < 10; i2++)
		{
			printf("\r\n2.TestCount: %d ->%d", TestCount++, i2);
			vTaskDelay(delayTime);
		}
		#if TYPE == 1
		taskEXIT_CRITICAL();
		#elif TYPE == 2
		xTaskResumeAll();
		#elif TYPE == 3
		ret = xSemaphoreGive(semaphrMutexHandle);
		vTaskDelay(delayTime);
		}
		#endif
	}
}

void createMutex(void)
{
	if (semaphrMutexHandle == NULL)
	{
		semaphrMutexHandle = xSemaphoreCreateMutex();
		if (semaphrMutexHandle != NULL)
			printf("\r\nm_ok");
		else
			printf("\r\nm_fail");
	}
}





