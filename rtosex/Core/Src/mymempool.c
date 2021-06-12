#include "mymempool.h"

/*************************************内存池的创建/使用示例**********************************************/
static rt_uint8_t *memptr[50];			// 申请到内存的指针数组
// 声明内存池控制块
rt_mp_t mp_create;
rt_timer_t releaseTimer = RT_NULL;
rt_thread_t requestMemThread = RT_NULL;
rt_thread_t releaseMemThread = RT_NULL;
int mempoolTaskCreate(void)
{
	mp_create = rt_mp_create("mp_create",			// 内存池池名称
														40,							// 内存块总数
														16);						// 每块内存块多大
	
	releaseTimer = rt_timer_create("releaseTimer",							// 定时器名称
																 releaseMemoryTimer,					// 定时时间到后执行的回调函数
																 RT_NULL,											// 回调函数参数
																 1000*30,											// 定时周期-时钟节拍，例如这里定义为1ms，1000则是1S
																 RT_TIMER_FLAG_PERIODIC				// 定时器标志：周期性
																);
	if (releaseTimer != RT_NULL)
	{
		rt_timer_start(releaseTimer);
	} else return 0;
	
	requestMemThread = rt_thread_create("requestMemThread",
																			requestMemoryThread_entery,
																			RT_NULL,
																			256,
																			3,
																			100);
	if (requestMemThread != RT_NULL)
	{
		rt_thread_startup(requestMemThread);
	} else return 0;
	
	releaseMemThread = rt_thread_create("releaseMemThread",
																			releaseMemoryThread_entery,
																			RT_NULL,
																			256,
																			5,
																			3);
	if (releaseMemThread != RT_NULL)
	{
		rt_thread_startup(releaseMemThread);
		return 1;
	}
	
	for (int i = 0; i < 50; i ++) memptr[i] = RT_NULL;

	return 0;
}
INIT_APP_EXPORT(mempoolTaskCreate);

//------------------------------------------------------------------------------------
// 静态-初始化内存池
static rt_uint8_t mempool[40*16];    // 内存池-40*16大小
struct rt_mempool mp_init;
struct rt_timer releaseTimer1;
struct rt_thread requestMemThread1;
char requestMemStack[256];
struct rt_thread releaseMemThread1;
char releaseMemStack[256];
int mempoolTaskInit(void)
{
	rt_err_t ret = rt_mp_init(&mp_init,
														"mp_init",
														&mempool[0],
														sizeof(mempool),
														16);
	
	rt_timer_init(&releaseTimer1,								// 定时器控制块指针
								"releaseTimer1",							// 定时器名称
								releaseMemoryTimer,						// 定时时间到后执行的回调函数
								RT_NULL,											// 回调函数参数
								1000*30,												// 定时周期-时钟节拍，例如这里定义为1ms，1000则是1S
								RT_TIMER_FLAG_PERIODIC				// 定时器标志：周期性
							 );
	rt_timer_start(&releaseTimer1);
	
	ret = rt_thread_init(&requestMemThread1,
												"requestMemThread1",
												requestMemoryThread_entery,
												RT_NULL,
												&requestMemStack[0],
												sizeof(requestMemStack),
												3,
												100);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&requestMemThread1);
	} else return 0;
	
	ret = rt_thread_init(&releaseMemThread1,
												"releaseMemThread1",
												releaseMemoryThread_entery,
												RT_NULL,
												&releaseMemStack[0],
												sizeof(releaseMemStack),
												5,
												3);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&releaseMemThread1);
		return 1;
	}
	for (int i = 0; i < 50; i ++) memptr[i] = RT_NULL;
	return 0;
}
//INIT_APP_EXPORT(mempoolTaskInit);

// 定时器回调函数-释放内存(归还内存池)
void releaseMemoryTimer(void* parameter)
{
	for (int i = 0; i < 50; i++)
	{
		if (memptr[i] != RT_NULL)
		{
			rt_mp_free(memptr[i]);
			memptr[i] = RT_NULL;
			rt_kprintf("\r\n release Memory No%d at Timer", i);
		}
	}
}
// 申请内存(从内存池)线程
void requestMemoryThread_entery(void* parameter)
{
	while(1)
	{
		for (int i = 0; i < 50; i++)
		{
			if (memptr[i] == RT_NULL)
			{
				#if 1  // 动态
				memptr[i] = rt_mp_alloc(mp_create, RT_WAITING_FOREVER);
				#else  // 静态
				memptr[i] = rt_mp_alloc(&mp_init, RT_WAITING_FOREVER);
				#endif
				if (memptr[i] != RT_NULL)
				{
					rt_kprintf("\r\n alloc Memory No.%d OK", i);
				}
			}
		}
		rt_thread_mdelay(1000*2);
	}
}
// 释放内存(归还内存池)线程
void releaseMemoryThread_entery(void* parameter)
{
	while(1)
	{
		rt_thread_mdelay(1000*10);
		for (int i = 0; i < 50; i++)
		{
			if (memptr[i] != RT_NULL)
			{
				rt_mp_free(memptr[i]);
				memptr[i] = RT_NULL;
				rt_kprintf("\r\n release Memory No%d at release Thread", i);
			}
		}
	}
}
/*************************************内存池的创建/使用示例**********************************************/


