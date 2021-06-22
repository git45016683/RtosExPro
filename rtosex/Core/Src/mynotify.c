#include "mynotify.h"

static TaskHandle_t task2StaticHandle = NULL;										// 静态线程句柄
static StackType_t task2StaticStack[configMINIMAL_STACK_SIZE];	// 静态线程栈大小
static StaticTask_t task2StaticTCB;															// 静态线程控制块
static TaskHandle_t task4StaticHandle = NULL;										// 静态线程句柄
static StackType_t task4StaticStack[configMINIMAL_STACK_SIZE];	// 静态线程栈大小
static StaticTask_t task4StaticTCB;															// 静态线程控制块
static TaskHandle_t task6StaticHandle = NULL;										// 静态线程句柄
static StackType_t task6StaticStack[configMINIMAL_STACK_SIZE];	// 静态线程栈大小
static StaticTask_t task6StaticTCB;															// 静态线程控制块


// 1-NoNotifyValueType[只发送通知,没通知值-信号量类型]  -> xTaskNotifyGive/xTaskNotifyTake
// 2-HasNotifyValueType[发送通知,带通知值-消息队列]  -> xTaskNotify/xTaskNotifyWait
// 3-HasNotifyValueAndReturnType[发送通知,带通知值,并返回通知值]  -> xTaskNotifyAndQuery/xTaskNotifyWait
#define NOTIFY_TYPE		1  


// 动态创建线程
static BaseType_t ret = NULL;
static TaskHandle_t task1Handle = NULL;		// 动态线程句柄
static TaskHandle_t task3Handle = NULL;		// 动态线程句柄
static TaskHandle_t task5Handle = NULL;		// 动态线程句柄
void TaskCreateForNotify(void)
{
	printf("\r\nTaskCreateForNotify");
	ret = xTaskCreate(NotifyTask1_entery,													// 任务入口函数
															"NotifyTask1_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&task1Handle);											// 任务句柄
	
	ret = xTaskCreate(NotifyTask3_entery,													// 任务入口函数
															"NotifyTask3_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&task3Handle);											// 任务句柄
	
	ret = xTaskCreate(NotifyTask5_entery,													// 任务入口函数
															"NotifyTask5_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&task5Handle);											// 任务句柄
}

static unsigned int task1TestCount = 0;
static char str[64] = "";
void NotifyTask1_entery(void* parameter)
{
	uint32_t send1 = 1; 
	uint32_t ret_num;
	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ*8);
		ret = xTaskNotifyGive(task2StaticHandle);
		printf("\r\n--NotifyTask1: %d, ret: %ld", task1TestCount++, ret);
	}
}
void NotifyTask3_entery(void* parameter)
{
	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ*6);
		ret = xTaskNotify(task4StaticHandle, 1, eIncrement);
		printf("\r\n--NotifyTask3: %d, ret: %ld", task1TestCount++, ret);
	}
}
void NotifyTask5_entery(void* parameter)
{
	uint32_t ret_num;
	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ*4);
		ret = xTaskNotifyAndQuery(task6StaticHandle, 5, eIncrement, &ret_num);
		printf("\r\n--NotifyTask5: %d, ret: %ld, ret_num:%d", task1TestCount++, ret, ret_num);
	}
}

////// 静态创建线程
void TaskCreateStaticForNotify(void)
{
	printf("\r\nTaskCreateStaticForNotify");
	task2StaticHandle = xTaskCreateStatic(NotifyTask2_entery,			// 任务入口函数
																			"NotifyTask2_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			task2StaticStack,					// 任务栈起始地址
																			&task2StaticTCB);					// 任务控制块
	
	task4StaticHandle = xTaskCreateStatic(NotifyTask4_entery,			// 任务入口函数
																			"NotifyTask4_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			task4StaticStack,					// 任务栈起始地址
																			&task4StaticTCB);					// 任务控制块
	
	task6StaticHandle = xTaskCreateStatic(NotifyTask6_entery,			// 任务入口函数
																			"NotifyTask6_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			task6StaticStack,					// 任务栈起始地址
																			&task6StaticTCB);					// 任务控制块
}

static unsigned int task2TestCount = 0;
void NotifyTask2_entery(void* parameter)
{
	while(1)
	{
		ret = ulTaskNotifyTake(pdTRUE, configTICK_RATE_HZ*10);
		printf("\r\nNotifyTask2: %d, ret: %ld", task2TestCount++, ret);
		vTaskDelay(configTICK_RATE_HZ);
	}
}

void NotifyTask4_entery(void* parameter)
{
	uint32_t comeIMG = 0;
	while(1)
	{
		ret = xTaskNotifyWait(pdFALSE, pdTRUE, &comeIMG, configTICK_RATE_HZ*8);
		printf("\r\nNotifyTask4: %d, ret %ld", task2TestCount++, ret);
		vTaskDelay(configTICK_RATE_HZ);
	}
}

void NotifyTask6_entery(void* parameter)
{
	uint32_t comeIMG = 0;
	while(1)
	{
		ret = xTaskNotifyWait(pdFALSE, pdTRUE, &comeIMG, configTICK_RATE_HZ*5);
		printf("\r\nNotifyTask6: %d, ret: %ld", task2TestCount++, ret);
		vTaskDelay(configTICK_RATE_HZ);
	}
}








