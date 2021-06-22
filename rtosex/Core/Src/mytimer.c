#include "mytimer.h"

static BaseType_t ret = pdFALSE;
// 动态创建定时器
static TimerHandle_t lifeTimerHandle = NULL;		// 声明周期定时器句柄
void CreateForLifeTimer(void)
{
	lifeTimerHandle = xTimerCreate("LifeTimer_entery",			// 定时器名
																 configTICK_RATE_HZ*5,		// 定时周期
																 pdTRUE,									// 周期性[pdTRUE]还是一次性[pdFALSE]
																 (void*)1,							// 定时器ID
																 LifeTimer_entery					// 定时器回调函数-执行实体
																 );
	if (lifeTimerHandle != NULL)
	{
		ret = xTimerStart(lifeTimerHandle, 0);  // 0-定时器start后多久执行
		if (ret != pdTRUE) printf("\r\nxTimerStart Failed");
	}
	else printf("\r\nCreateForLifeTimer Failed");
}

static unsigned int task1TestCount = 0;
void LifeTimer_entery(TimerHandle_t xTimer)
{
	printf("\r\nrunning %d S.", (++task1TestCount)*5);
}

////// 静态创建线程
static TimerHandle_t testTimerHandle = NULL;		// 测试定时器句柄
static StaticTimer_t testTimerBuff[64];  
void CreateStaticForTestTimer(void)
{
	testTimerHandle = xTimerCreateStatic("TestTimer_entery",			// 定时器名称
																			configTICK_RATE_HZ*36,		// 定时器周期
																			pdFALSE,									// 一次性定时器[pdFALSE]
																			(void*)0,									// 定时器ID
																			TestTimer_entery,					// 定时器回调函数-执行实体
																			testTimerBuff							// 定时器状态缓存
																			);
	if (testTimerHandle != NULL)
	{
		ret = xTimerStart(testTimerHandle, 10);  // 10-定时器start后多久执行
		if (ret != pdTRUE) printf("\r\nxTimerStart Failed For Static");
	}
	else printf("\r\nCreateStaticForTestTimer Failed");
}

static unsigned int task2TestCount = 0;
void TestTimer_entery(TimerHandle_t xTimer)
{
	printf("\r\n I am an Oneshot testTimer, Now My Timeout, Googbye...");
}








