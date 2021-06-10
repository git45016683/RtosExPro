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
INIT_APP_EXPORT(SampleTaskCreate);

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
rt_mutex_t s;

/*************************************互斥量的创建/使用示例**********************************************/


