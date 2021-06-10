#include "mymutex.h"

// 未使用互斥量进行线程间同步的线程
rt_thread_t sampleThread1;
rt_thread_t sampleThread2;
int SampleTaskCreate(void)
{
	sampleThread1 = rt_thread_create("sampleThread1",
                             AddThread1_entery,
                             RT_NULL,
                             256,
                             4,
                             10);
	if (sampleThread1 != RT_NULL)
	{
		rt_thread_startup(sampleThread1);
	}
	else
	{
		return 0;
	}
	
	sampleThread2 = rt_thread_create("sampleThread2",
																		AddThread2_entery,
																		RT_NULL,
																		256,
																		2,
																		10);
	if (sampleThread2 != RT_NULL)
	{
		rt_thread_startup(sampleThread2);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(SampleTaskCreate);

static uint8_t number1, number2 = 0;
void AddThread1_entery(void* parameter)
{
	while(1)
	{
		number1++;
		rt_thread_mdelay(100);
		number2++;
	}
}

void AddThread2_entery(void* parameter)
{
	while(1)
	{
		if (number1 != number2)
		{
			rt_kprintf("\r\nnumer1(%d) != number2(%d)", number1, number2);
		} 
		else
		{
			rt_kprintf("\r\nnumer1 == number2, is %d", number1);
		}
		number1++;
		number2++;
		rt_thread_mdelay(1000);
	}
}


/*************************************互斥量的创建/使用示例**********************************************/
// 声明互斥量
rt_mutex_t mutex_create_sync = RT_NULL;
rt_thread_t sampleThread11;
rt_thread_t sampleThread22;
int SampleWithMutexTaskCreate(void)
{
	mutex_create_sync = rt_mutex_create("mutex_create_sync", RT_IPC_FLAG_FIFO);
	if (mutex_create_sync != RT_NULL)
	{
		rt_kprintf("\r\ncreate mutex success");
	}
	
	sampleThread11 = rt_thread_create("sampleThread11",
                             AddThread11_entery,
                             RT_NULL,
                             256,
                             4,
                             10);
	if (sampleThread11 != RT_NULL)
	{
		rt_thread_startup(sampleThread11);
	}
	else
	{
		return 0;
	}
	
	sampleThread22 = rt_thread_create("sampleThread22",
																		AddThread22_entery,
																		RT_NULL,
																		256,
																		2,
																		10);
	if (sampleThread22 != RT_NULL)
	{
		rt_thread_startup(sampleThread22);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(SampleWithMutexTaskCreate);

extern struct rt_mutex mutex_init_sync;
static uint8_t number11, number22 = 0;
void AddThread11_entery(void* parameter)
{
	while(1)
	{
		#if 0  // 动态
		if (RT_EOK == rt_mutex_take(mutex_create_sync, RT_WAITING_FOREVER))  // 一直等待，阻塞在此直到获取互斥量
		#else
		if (RT_EOK == rt_mutex_take(&mutex_init_sync, RT_WAITING_FOREVER))  // 一直等待，阻塞在此直到获取互斥量
		#endif
		{
			number11++;
			rt_thread_mdelay(100);
			number22++;
			
			#if 0  // 动态
			if (RT_EOK != rt_mutex_release(mutex_create_sync))  // 释放互斥量
			#else
			if (RT_EOK != rt_mutex_release(&mutex_init_sync))  // 释放互斥量
			#endif
			{
				rt_kprintf("\r\nmutex_create_sync release failed");
			}
		}
	}
}

void AddThread22_entery(void* parameter)
{
	while(1)
	{
		#if 0  // 动态
		if (RT_EOK == rt_mutex_take(mutex_create_sync, RT_WAITING_FOREVER))  // 一直等待，阻塞在此直到获取互斥量
		#else
		if (RT_EOK == rt_mutex_take(&mutex_init_sync, RT_WAITING_FOREVER))  // 一直等待，阻塞在此直到获取互斥量
		#endif
		{
			if (number11 != number22)
			{
				rt_kprintf("\r\nnumer11(%d) != number22(%d)", number11, number22);
			} 
			else
			{
				rt_kprintf("\r\nnumer11 == number22, is %d", number11);
			}
			number11++;
			number22++;
			rt_thread_mdelay(1000);
		
			#if 0  // 动态
			if (RT_EOK != rt_mutex_release(mutex_create_sync))  // 释放互斥量
			#else
			if (RT_EOK != rt_mutex_release(&mutex_init_sync))  // 释放互斥量
			#endif
			{
				rt_kprintf("\r\nmutex_create_sync release failed");
			}
		}
	}
}

//------------------------------------------------------------------------------------
// 静态创建互斥量
struct rt_mutex mutex_init_sync;
struct rt_thread sampleThread111;
static char thread111Stack[256];
struct rt_thread sampleThread222;
static char thread222Stack[256];
int SampleWithMutexTaskInit(void)
{
	rt_err_t ret = rt_mutex_init(&mutex_init_sync,
															 "mutex_init_sync",
															 RT_IPC_FLAG_FIFO);
	if (ret == RT_EOK)
	{
		rt_kprintf("\r\nmutex_init_sync init success");
	} else return 0;
	
	ret = rt_thread_init(&sampleThread111,
											 "sampleThread111",
											 AddThread11_entery,
											 RT_NULL,
											 &thread111Stack[0],
											 sizeof(thread111Stack),
											 4,
											 10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&sampleThread111);
	} else return 0;
	
	ret = rt_thread_init(&sampleThread222,
												"sampleThread222",
												AddThread22_entery,
												RT_NULL,
												&thread222Stack[0],
												sizeof(thread222Stack),
												2,
												10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&sampleThread222);
		return 1;
	}
	return 0;
}
INIT_APP_EXPORT(SampleWithMutexTaskInit);
/*************************************互斥量的创建/使用示例**********************************************/


