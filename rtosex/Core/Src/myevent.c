#include "myevent.h"

static EventGroupHandle_t eventHandle;  // 事件句柄
#define KEY_1  (1<<0)
#define KEY_6  (1<<5)
#define KEY_3  (1<<3)

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
static TaskHandle_t task3Handle = NULL;		// 动态线程句柄
void TaskCreateForEvent(void)
{
	printf("\r\nTaskCreateForEvent");
	BaseType_t ret = xTaskCreate(EventTask1_entery,								// 任务入口函数
															"EventTask1_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&taskHandle);											// 任务句柄
	ret = xTaskCreate(EventTask3_entery,													// 任务入口函数
															"EventTask3_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&task3Handle);										// 任务句柄
}

static unsigned int task1TestCount = 0;
static char str[64] = "";
void EventTask1_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	static BaseType_t ret = NULL;
	static char str[64] = "";
	while(1)
	{
		if (eventHandle == NULL)
		{
			printf("\r\n--task1TestCount: %d, %x", task1TestCount++, *(uint32_t*)eventHandle);
			vTaskDelay(configTICK_RATE_HZ*8);
		} else 
		{
			vTaskDelay(configTICK_RATE_HZ*3);
			xEventGroupSetBits(eventHandle, KEY_1);
			printf("\r\n--%x, set Event Bits (%08x) ret: %ld", *(uint32_t*)eventHandle, KEY_1, ret);
			vTaskDelay(configTICK_RATE_HZ*3);
			xEventGroupSetBits(eventHandle, KEY_3);
			printf("\r\n--%x, set Event Bits (%08x) ret: %ld", *(uint32_t*)eventHandle, KEY_3, ret);
			vTaskDelay(configTICK_RATE_HZ*3);
			xEventGroupSetBits(eventHandle, KEY_6);
			printf("\r\n--%x, set Event Bits (%08x) ret: %ld", *(uint32_t*)eventHandle, KEY_6, ret);
		}
	}
}

void EventTask3_entery(void* parameter)
{
	static unsigned int task3TestCount = 0;
	static BaseType_t ret = NULL;
	EventBits_t ret_event = 0;
	static uint64_t count1 = 0;
	static uint64_t count6 = 0;
	while(1)
	{
		if (eventHandle == NULL)
		{
			printf("\r\ntask3TestCount: %d", task3TestCount++);
			vTaskDelay(configTICK_RATE_HZ);
		} else 
		{
			ret_event =xEventGroupWaitBits(eventHandle, KEY_1 | KEY_3, pdFALSE, pdFALSE, 1000*10);
			if (ret_event & KEY_3)  // 收到3
			{if (++count6%50000 == 0) printf("\r\ntask3TestCount: %d -> event: %08x", task3TestCount++, KEY_3);
			} else if (ret_event & KEY_1)  // 收到1
			{if (++count1%50000 == 0) printf("\r\ntask3TestCount: %d -> event: %08x", task3TestCount++, KEY_1);
			} else printf("\r\ntask3TestCount: %d -> event ret: %ld", task3TestCount++, ret);
		}
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStaticForEvent(void)
{
	printf("\r\nTaskCreateStaticForEvent");
	taskStaticHandle = xTaskCreateStatic(EventTask2_entery,				// 任务入口函数
																			"EventTask2_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			3,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}


void EventTask2_entery(void* parameter)
{
	static unsigned int task2TestCount = 0;
	static BaseType_t ret = NULL;
	char recvStr[128] = "";
	EventBits_t ret_event = 0;
	while(1)
	{
		if (eventHandle == NULL)
		{
			printf("\r\ntask2TestCount: %d", task2TestCount++);
			vTaskDelay(configTICK_RATE_HZ);
		} else 
		{
			printf("\r\n----------------");
			ret_event =xEventGroupWaitBits(eventHandle, KEY_1 | KEY_6, pdTRUE, pdTRUE, portMAX_DELAY);
			if (ret_event & (KEY_1|KEY_6))  // 收到1、6
			{printf("\r\ntask2TestCount: %d -> event: %08x & %08x", task2TestCount++, KEY_1, KEY_6);
			} else if (ret_event & KEY_6)  // 收到6
			{printf("\r\ntask2TestCount: %d -> event: %08x", task2TestCount++, KEY_6);
			} else if (ret_event & KEY_1)  // 收到1
			{printf("\r\ntask2TestCount: %d -> event: %08x", task2TestCount++, KEY_1);
			} else printf("\r\ntask2TestCount: %d -> event ret: %ld", task2TestCount++, ret);
		}
	}
}


void createEvent(void)
{
	if (eventHandle == NULL)
	{
		eventHandle = xEventGroupCreate();
	}
}
//void createCountingSemaph(void)
//{
//	if (eventHandle == NULL)
//	{
//		eventHandle = xSemaphoreCreateCounting(10, 0);
//	}
//}
//void sendBinarySemphFromISR(void)
//{
//	BaseType_t pxHigherPriorityTaskWoken = 1;
//	BaseType_t ret = NULL;
//	taskENTER_CRITICAL();
//	
//	sprintf(str, "binarySemphrSendISR -> %d", task1TestCount++);
//	ret = xSemaphoreGiveFromISR(eventHandle, &pxHigherPriorityTaskWoken);
//	
//	printf("\r\n--%s", str);
//	taskEXIT_CRITICAL();
//}

//void sendCountingSemphFromISR(uint8_t send_type)
//{
//	BaseType_t pxHigherPriorityTaskWoken = 1;
//	BaseType_t ret = NULL;
//	taskENTER_CRITICAL();
//	
//	sprintf(str, "%d.xQueueSendISR -> %d", send_type, task1TestCount++);
//	
//	printf("\r\n--%s", str);
//	taskEXIT_CRITICAL();
//}





