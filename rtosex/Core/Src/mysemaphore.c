#include "mysemaphore.h"

static xSemaphoreHandle semaphHandle;  // 信号量句柄
// 1-BinarySemaphore--二值信号量
// 2-CountingSemaphore--计数信号量
#define SEMPHR_TYPE		2  // 1-BinarySemaphore  2-CountingSemaphore  


// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
void TaskCreateForSemaphore(void)
{
	#if SEMPHR_TYPE == 1  // 二值信号量
	createBinarySemaph();
	#elif SEMPHR_TYPE == 2  // 计数信号量
	createCountingSemaph();
	#endif
	printf("\r\nTaskCreateForSemaphore");
	BaseType_t ret = xTaskCreate(SemaphTask1_entery,							// 任务入口函数
															"SemaphTask1_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&taskHandle);											// 任务句柄
}

static unsigned int task1TestCount = 0;
static char str[64] = "";
void SemaphTask1_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	static BaseType_t ret = NULL;
	static char str[64] = "";
	while(1)
	{
		if (semaphHandle == NULL)
		{
			printf("\r\n--task1TestCount: %d, %x", task1TestCount++, *(uint32_t*)semaphHandle);
			vTaskDelay(configTICK_RATE_HZ*8);
		} else 
		{
			#if SEMPHR_TYPE == 1  // 二值信号量
			ret = xSemaphoreGive(semaphHandle);
//			ret = xSemaphoreGive(semaphHandle);
//			ret = xSemaphoreGive(semaphHandle);
//			ret = xSemaphoreGive(semaphHandle);
//			ret = xSemaphoreGive(semaphHandle);
			#elif SEMPHR_TYPE == 2  // 计数信号量
			ret = xSemaphoreGive(semaphHandle);
			ret = xSemaphoreGive(semaphHandle);
			ret = xSemaphoreGive(semaphHandle);
			ret = xSemaphoreGive(semaphHandle);
			ret = xSemaphoreGive(semaphHandle);
			ret = xSemaphoreGive(semaphHandle);
			#endif
			printf("\r\n--%x, give semaphore ret: %ld", *(uint32_t*)semaphHandle, ret);
			vTaskDelay(configTICK_RATE_HZ*11);
		}
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStaticForSemaphore(void)
{
	printf("\r\nTaskCreateStaticForSemaphore");
	taskStaticHandle = xTaskCreateStatic(SemphTask2_entery,				// 任务入口函数
																			"SemphTask2_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}


void SemphTask2_entery(void* parameter)
{
	static unsigned int task2TestCount = 0;
	static BaseType_t ret = NULL;
	char recvStr[128] = "";
	while(1)
	{
		if (semaphHandle == NULL)
		{
			printf("\r\ntask2TestCount: %d", task2TestCount++);
			vTaskDelay(configTICK_RATE_HZ);
		} else 
		{
			#if SEMPHR_TYPE == 1  // 二值信号量
			ret = xSemaphoreTake(semaphHandle, 1000*5);
			#elif SEMPHR_TYPE == 2  // 计数信号量
			ret = xSemaphoreTake(semaphHandle, 800);
			#endif
			printf("\r\ntask2TestCount: %d -> takeSemaphore ret: %ld", task2TestCount++, ret);
		}
	}
}


void createBinarySemaph(void)
{
	if (semaphHandle == NULL)
	{
		vSemaphoreCreateBinary(semaphHandle);
	}
}
void createCountingSemaph(void)
{
	if (semaphHandle == NULL)
	{
		semaphHandle = xSemaphoreCreateCounting(10, 0);
	}
}
void sendBinarySemphFromISR(void)
{
	BaseType_t pxHigherPriorityTaskWoken = 1;
	BaseType_t ret = NULL;
	taskENTER_CRITICAL();
	
	sprintf(str, "binarySemphrSendISR -> %d", task1TestCount++);
	ret = xSemaphoreGiveFromISR(semaphHandle, &pxHigherPriorityTaskWoken);
	
	printf("\r\n--%s", str);
	taskEXIT_CRITICAL();
}

void sendCountingSemphFromISR(uint8_t send_type)
{
	BaseType_t pxHigherPriorityTaskWoken = 1;
	BaseType_t ret = NULL;
	taskENTER_CRITICAL();
	
	sprintf(str, "%d.xQueueSendISR -> %d", send_type, task1TestCount++);
	
	printf("\r\n--%s", str);
	taskEXIT_CRITICAL();
}





