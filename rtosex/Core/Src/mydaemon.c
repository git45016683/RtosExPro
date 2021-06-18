#include "mydaemon.h"

#define delayTime 1000
#define TIMEOUT  10

// 访问守护任务资源的队列声明
static xQueueHandle daemonQueueHandle = NULL;

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
void TaskCreateForDaemon(void)
{
	printf("\r\nTaskCreateForSemaphore");
	BaseType_t ret = xTaskCreate(DaemonTask1_entery,						// 任务入口函数
															"DaemonTask1_entery",						// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															1,																// 任务优先级
															&taskHandle);											// 任务句柄
	
	ret = xTaskCreate(DaemonTask3_entery,												// 任务入口函数
															"DaemonTask3_entery",						// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															3,																// 任务优先级
															&taskHandle);											// 任务句柄
}

static unsigned int TestCount = 0;
void DaemonTask1_entery(void* parameter)
{
	BaseType_t ret = NULL;
	char str[64] = "";
	while(1)
	{
		for(int i1 = 0; i1 < 10; i1++)
		{
			sprintf(str, "%d-%d", 1, 1);
			xQueueSend(daemonQueueHandle, str, 100);
//			printf("\r\n1....%s", str);
		}
		vTaskDelay(delayTime);
	}
}
void DaemonTask3_entery(void* parameter)
{
	BaseType_t ret = NULL;
	char str[64] = "";
	while(1)
	{
		for(int i3 = 0; i3 < 10; i3++)
		{
			sprintf(str, "%d-%d", 3, 1);
			xQueueSend(daemonQueueHandle, str, 100);
//			printf("\r\n3....%s", str);
		}
		vTaskDelay(delayTime);
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
// 守护任务
static TaskHandle_t daemonStaticHandle = NULL;										// 静态线程句柄
static StackType_t daemonStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t daemonStaticTCB;															// 静态线程控制块
void TaskCreateStaticForDaemon(void)
{
	printf("\r\nTaskCreateStaticForSemaphore");
	taskStaticHandle = xTaskCreateStatic(DaemonTask2_entery,		// 任务入口函数
																			"DaemonTask2_entery",		// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			2,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
	
	daemonStaticHandle = xTaskCreateStatic(Daemon_entery,		// 任务入口函数
																			"Daemon_entery",		// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			daemonStaticStack,					// 任务栈起始地址
																			&daemonStaticTCB);					// 任务控制块
}

void DaemonTask2_entery(void* parameter)
{
	BaseType_t ret = NULL;
	char str[64] = "";
	while(1)
	{
		for(int i2 = 0; i2 < 10; i2++)
		{
			sprintf(str, "%d-%d", 2, 1);
			xQueueSend(daemonQueueHandle, str, 100);
//			printf("\r\n2....%s", str);
		}
		vTaskDelay(delayTime);
	}
}

void Daemon_entery(void* parameter)
{
	char cStr[32] = "";
	char str[64] = "";
	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ);
		xQueueReceive(daemonQueueHandle, cStr, portMAX_DELAY);
//		char cNo = cStr[0];
//		int i = atoi(&cStr[1]);
//		sprintf(str, "%d.TestCount: %s", cNo, &cStr[1]);
		printf("\r\n%s", cStr);
	}
}

void createDaemonQueue(void)
{
	if (daemonQueueHandle == NULL)
	{
		daemonQueueHandle = xQueueCreate(5, sizeof(char*));  // 创建一个深度未5的队列，数据类型未int
	}
}




