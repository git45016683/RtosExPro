#include "mysemaphore.h"

static xQueueHandle queueHandle = NULL;  // 消息队列句柄
// 1-xQueueSend--插入到队末
// 2-xQueueSendToBack--插入到队末
// 3-xQueueOverwrite--直接写入队末
// 3-xQueueSendToFront--写入队首
#define SEND_TYPE		4  // 1-xQueueSend  2-xQueueSendToBack  3-xQueueOverwrite 4-xQueueSendToFront
// 1-xQueueReceive--取出消息，并从队列中删除该消息
// 2-xQueuePeek--取出消息，不删除队列中的该消息
#define RECV_TYPE		1  // 1-xQueueReceive  2-xQueuePeek  3-xQueueOverWrite

// 动态创建线程
static TaskHandle_t taskHandle = NULL;		// 动态线程句柄
void TaskCreateForQueue(void)
{
	printf("\r\nTaskCreateForQueue");
	BaseType_t ret = xTaskCreate(QueueTask1_entery,								// 任务入口函数
															"QueueTask1_entery",							// 任务名称
															configMINIMAL_STACK_SIZE,					// 任务栈大小
															NULL,															// 任务入口函数参数
															2,																// 任务优先级
															&taskHandle);											// 任务句柄
}

static unsigned int task1TestCount = 0;
static char str[64] = "";
void QueueTask1_entery(void* parameter)
{
	static unsigned int task1TestCount = 0;
	static BaseType_t ret = NULL;
	static char str[64] = "";
	while(1)
	{
		if (queueHandle == NULL)
		{
			printf("\r\n--task1TestCount: %d", task1TestCount++);
			if (task1TestCount == 1)  // 8S后创建消息队列
			{
				createMyQueue();
			}
		} else
		{
			sprintf(str, "xQueueSend -> %d", task1TestCount++);
			#if SEND_TYPE == 1
			ret = xQueueSend(queueHandle, str, 100);  // 写入到队末，队列满后，超时不会插入
			#elif SEND_TYPE == 2
			ret = xQueueSendToBack(queueHandle, str, 100);  // 写入到队末，队列满后，超时不会插入
			#elif SEND_TYPE == 3
			ret = xQueueOverwrite(queueHandle, str);
			#elif SEND_TYPE == 4
			ret = xQueueSendToFront(queueHandle, str, 100);  // 写入到队首，队列满后，超时不会插入
			#endif
//			printf("\r\n--%s", str);
		}
		vTaskDelay(configTICK_RATE_HZ/10);
	}
}

////// 静态创建线程
static TaskHandle_t taskStaticHandle = NULL;										// 静态线程句柄
static StackType_t taskStaticStack[configMINIMAL_STACK_SIZE];		// 静态线程栈大小
static StaticTask_t taskStaticTCB;															// 静态线程控制块
void TaskCreateStaticForQueue(void)
{
	printf("\r\nTaskCreateStatic");
	taskStaticHandle = xTaskCreateStatic(QueueTask2_entery,				// 任务入口函数
																			"QueueTask2_entery",			// 任务名称
																			configMINIMAL_STACK_SIZE,	// 任务栈大小
																			NULL,											// 任务入口函数参数
																			1,												// 任务优先级
																			taskStaticStack,					// 任务栈起始地址
																			&taskStaticTCB);					// 任务控制块
}


void QueueTask2_entery(void* parameter)
{
	static unsigned int task2TestCount = 0;
	static BaseType_t ret = NULL;
	char recvStr[128] = "";
	while(1)
	{
		if (queueHandle == NULL)
		{
			printf("\r\n[task2TestCount: %d", task2TestCount++);
			vTaskDelay(configTICK_RATE_HZ);
		} else 
		{
			#if RECV_TYPE == 1
			ret = xQueueReceive(queueHandle, recvStr, 1000);  // 取出并删除消息
			#elif RECV_TYPE == 2
			ret = xQueuePeek(queueHandle, recvStr, 1000);  // 取出不删除消息
			#endif
			if (recvStr[0] != '\0')
			{
				#if RECV_TYPE == 1
				printf("\r\n %d.recvQueue msg: %s, and reset receive buff", task2TestCount++, recvStr);
				memset(recvStr, '\0', sizeof(recvStr));
				vTaskDelay(configTICK_RATE_HZ);
				#elif RECV_TYPE == 2 && (SEND_TYPE == 3 || SEND_TYPE == 4 || SEND_TYPE == 2)
				printf("\r\n %d.recvQueue msg: %s, and reset receive buff", task2TestCount++, recvStr);
				memset(recvStr, '\0', sizeof(recvStr));
				vTaskDelay(configTICK_RATE_HZ*3);
				#elif RECV_TYPE == 2
				printf("\r\n %d.recvQueue msg: %s, and reset receive buff", task2TestCount++, recvStr);
				memset(recvStr, '\0', sizeof(recvStr));
				vTaskDelay(configTICK_RATE_HZ);
//				if (task2TestCount%3 == 0)
//				{
//					ret = xQueueReceive(queueHandle, recvStr, 1000);  // 取出并删除消息
//					printf("\r\n %d.recvQueue msg: %s, and reset receive buff, delete msg", task2TestCount++, recvStr);
//				}
				#endif
			} else 
			{
				printf("\r\n %d.recvQueue NULL msg: %s.", task2TestCount, recvStr);
			}
		}
	}
}


void createMyQueue(void)
{
	if (queueHandle == NULL)
	{
		queueHandle = xQueueCreate(10, 64);  // 创建消息队列
	}
}
void deleteMyQueue(void)
{
	if (queueHandle != NULL)
	{
		vQueueDelete(queueHandle);
	}
}
void sendMyQueueFromISR(uint8_t send_type)
{
	BaseType_t pxHigherPriorityTaskWoken = 1;
	BaseType_t ret = NULL;
	taskENTER_CRITICAL();
	
	sprintf(str, "%d.xQueueSendISR -> %d", send_type, task1TestCount++);
	if (send_type == 5)
		ret = xQueueSendFromISR(queueHandle, str, &pxHigherPriorityTaskWoken);  // 写入到队末，队列满后，超时不会插入
	if (send_type == 6)
		ret = xQueueSendToBackFromISR(queueHandle, str, &pxHigherPriorityTaskWoken);  // 写入到队末，队列满后，超时不会插入
	if (send_type == 7)
		ret = xQueueOverwriteFromISR(queueHandle, str, NULL);
	if (send_type == 8)
		ret = xQueueSendToFrontFromISR(queueHandle, str, NULL);  // 写入到队首，队列满后，超时不会插入
	
	printf("\r\n--%s", str);
	taskEXIT_CRITICAL();
}


