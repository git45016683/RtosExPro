#include "mymsgqueue.h"

/*************************************消息队列的创建/使用示例**********************************************/
// 声明消息队列指针
rt_mq_t mq_create = RT_NULL;
rt_thread_t sendmsgThread;  // msg发送线程
rt_thread_t recvmsgThread;  // msg接收线程
int msgQueueboxTaskCreate(void)
{
	mq_create = rt_mq_create("mq_create",  					// 消息队列名
														32, 								 	// 消息队列中消息的最大长度
														10,										// 消息队列的最大个数
														RT_IPC_FLAG_FIFO);		// 消息队列取出的顺序-先进先出
	if (mq_create != RT_NULL)
	{
		rt_kprintf("\r\ncreate mq_create success");
	} else rt_kprintf("\r\ncreate mq_create failed!!!");
	
	sendmsgThread = rt_thread_create("sendmsgThread",
																		sendMsg_entery,
																		RT_NULL,
																		256,
																		4,
																		40);
	if (sendmsgThread != RT_NULL)
	{
		rt_thread_startup(sendmsgThread);
	} else return 0;
	
	recvmsgThread = rt_thread_create("recvmsgThread",
																		recvMsg_entery,
																		RT_NULL,
																		256,
																		4,
																		40);
	if (recvmsgThread != RT_NULL)
	{
		rt_thread_startup(recvmsgThread);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(msgQueueboxTaskCreate);

//------------------------------------------------------------------------------------
// 静态创建消息队列
struct rt_messagequeue mq_init;
rt_uint8_t  msgpool[32*10];
struct rt_thread sendmsgThread1;
static char sendmsgStack[256];
struct rt_thread recvmsgThread1;
static char recvmsgStack[256];
int msgQueueTaskInit(void)
{
	rt_err_t ret = rt_mq_init(&mq_init,
														"mq_init",
														&msgpool[0],
														32,
														sizeof(msgpool),
														RT_IPC_FLAG_FIFO);
	if (ret == RT_EOK)
	{
		rt_kprintf("\r\nmq_init init success");
	} else return 0;
	
	ret = rt_thread_init(&sendmsgThread1,
												"sendmsgThread1",
												sendMsg_entery,
												RT_NULL,
												&sendmsgStack[0],
												sizeof(sendmsgStack),
												4,
												40);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&sendmsgThread1);
	} else return 0;
	
	ret = rt_thread_init(&recvmsgThread1,
												"recvmsgThread1",
												recvMsg_entery,
												RT_NULL,
												&recvmsgStack[0],
												sizeof(recvmsgStack),
												4,
												40);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&recvmsgThread1);
		return 1;
	}
	return 0;
}
INIT_APP_EXPORT(msgQueueTaskInit);


// 示例线程
#define DYNAMIC 0
extern void errinfo(int index, rt_err_t err);
void sendMsg_entery(void* parameter)
{
	while(1)
	{
		#if DYNAMIC  // 动态
		rt_mq_send(mq_create, "xxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxx"));
		#else  // 静态
		rt_mq_send(&mq_init, "xxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxx"));
		#endif
		rt_thread_mdelay(5000);
	}
}

void recvMsg_entery(void* parameter)
{
	rt_err_t ret = RT_EOK;
	uint8_t msg[32];
	while(1)
	{
		#if DYNAMIC  // 动态
		ret = rt_mq_recv(mq_create,
										msg,
										sizeof(msg),
										1000);
		#else  // 静态
		ret = rt_mq_recv(&mq_init,
										msg,
										sizeof(msg),
										1000);
		#endif
		if (ret == RT_EOK)
		{
			rt_kprintf("\r\n1.receive a message: %s", msg);
		} else errinfo(2, ret);

		#if DYNAMIC  // 动态
		ret = rt_mq_recv(mq_create,
										msg,
										sizeof(msg),
										RT_WAITING_FOREVER);
		#else  // 静态
		ret = rt_mq_recv(&mq_init,
										msg,
										sizeof(msg),
										RT_WAITING_FOREVER);
		#endif
		if (ret == RT_EOK)
		{
			rt_kprintf("\r\n2.receive a message: %s", msg);
		} else errinfo(3, ret);
	}
}
/*************************************消息队列的创建/使用示例**********************************************/


