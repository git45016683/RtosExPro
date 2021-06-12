#include "mytimer.h"

/*************************************定时器的创建/使用示例**********************************************/
// 声明定时器控制块
rt_timer_t lifelogTimer = RT_NULL;
rt_timer_t testTimer = RT_NULL;
static rt_uint32_t lifeRunXs;
int timerTaskCreate(void)
{
	lifelogTimer = rt_timer_create("lifelogTimer",							// 定时器名称
																 lifeLogTimerOut,							// 定时时间到后执行的回调函数
																 (void*)&lifeRunXs,						// 回调函数参数
																 1000,												// 定时周期-时钟节拍，例如这里定义为1ms，1000则是1S
																 RT_TIMER_FLAG_PERIODIC				// 定时器标志：周期性
																);
	if (lifelogTimer != RT_NULL)
	{
		rt_timer_start(lifelogTimer);
	} else return 0;
	
	testTimer = rt_timer_create("testTimer",
															testTimerOut,
															RT_NULL,
															1000*36,
															RT_TIMER_FLAG_ONE_SHOT					// 定时器标识：一次性
															);
	if (testTimer != RT_NULL)
	{
		rt_timer_start(testTimer);
		return 1;
	}
	lifeRunXs = 0;
	return 0;
}
INIT_APP_EXPORT(timerTaskCreate);

//------------------------------------------------------------------------------------
// 静态-初始化定时器
struct rt_timer lifelogTimer1;
struct rt_timer testTimer1;
int timerTaskInit(void)
{
	rt_timer_init(&lifelogTimer1,								// 定时器控制块指针
								"lifelogTimer1",							// 定时器名称
								lifeLogTimerOut,							// 定时时间到后执行的回调函数
								(void*)&lifeRunXs,						// 回调函数参数
								1000,													// 定时周期-时钟节拍，例如这里定义为1ms，1000则是1S
								RT_TIMER_FLAG_PERIODIC				// 定时器标志：周期性
							 );
	rt_timer_start(&lifelogTimer1);
	
	rt_timer_init(&testTimer1,
								"testTimer1",
								testTimerOut,
								RT_NULL,
								1000*36,
								RT_TIMER_FLAG_ONE_SHOT);			// 定时器标识：一次性
	rt_timer_start(&testTimer1);
	return 0;
}
//INIT_APP_EXPORT(timerTaskInit);


// 定时器回调函数
void lifeLogTimerOut(void* parameter)
{
	if (0 == (++(*(uint32_t*)(parameter))%10))
	{
		rt_kprintf("\r\nrunning %d S.", *(uint32_t*)(parameter));
	}
}

void testTimerOut(void* parameter)
{
	rt_kprintf("\r\n I am an Oneshot testTimer, Now My Timeout, Googbye...");
}


// 官方示例高精度延时
void rt_hw_us_delay(rt_uint32_t us)
{
	rt_uint32_t ticks;
	rt_uint32_t told, tnow, tcnt = 0;
	rt_uint32_t reload = SysTick->LOAD;
	
	// 获得延时经过的 tick 数
	ticks = us * reload / (1000*1000 / RT_TICK_PER_SECOND);
	// 获得当前时间
	told = SysTick->VAL;
	
	while(1)
	{
		// 循环获得当前时间，直到达到指定的时间后退出循环
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
			{
				tcnt += told - tnow;
			}
			else
			{
				tcnt += reload - tnow - told;
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
}
/*************************************定时器的创建/使用示例**********************************************/


