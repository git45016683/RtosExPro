#include "myevent.h"
#include <stdlib.h>

/*************************************事件的创建/使用示例**********************************************/
// 声明事件集
rt_event_t div_event_create = RT_NULL;
rt_thread_t randDividerThread;  // 随机除数线程
rt_thread_t randDividendThread;  // 随机被除数线程
rt_thread_t DivisionThread;  // 除法运算线程
int DivTaskCreate(void)
{
	div_event_create = rt_event_create("div_event_create", RT_IPC_FLAG_FIFO);
	if (div_event_create != RT_NULL)
	{
		rt_kprintf("\r\ncreate div_event_create success");
	}
	
	randDividerThread = rt_thread_create("randDividerThread",
                             DividerOK_entery,
                             RT_NULL,
                             256,
                             4,
                             10);
	if (randDividerThread != RT_NULL)
	{
		rt_thread_startup(randDividerThread);
	} else return 0;
	
	randDividendThread = rt_thread_create("randDividendThread",
                             DividendOK_entery,
                             RT_NULL,
                             256,
                             4,
                             10);
	if (randDividendThread != RT_NULL)
	{
		rt_thread_startup(randDividendThread);
	} else return 0;
	
	DivisionThread = rt_thread_create("DivisionThread",
																		Division_entery,
																		RT_NULL,
																		256,
																		4,
																		40);
	if (DivisionThread != RT_NULL)
	{
		rt_thread_startup(DivisionThread);
		return 1;
	}
	return 0;
}
INIT_APP_EXPORT(DivTaskCreate);

//------------------------------------------------------------------------------------
// 静态创建事件
struct rt_event div_event_init;
struct rt_thread randDividerThread1;
static char randDividerStack[256];
struct rt_thread randDividendThread1;
static char randDividendStack[256];
struct rt_thread DivisionThread1;
static char divisionStack[256];
int DivTaskInit(void)
{
	rt_err_t ret = rt_event_init(&div_event_init,
															 "div_event_init",
															 RT_IPC_FLAG_FIFO);
	if (ret == RT_EOK)
	{
		rt_kprintf("\r\ndiv_event_init init success");
	} else return 0;
	
	ret = rt_thread_init(&randDividerThread1,
											 "randDividerThread1",
											 DividerOK_entery,
											 RT_NULL,
											 &randDividerStack[0],
											 sizeof(randDividerStack),
											 4,
											 10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&randDividerThread1);
	} else return 0;
	
	ret = rt_thread_init(&randDividendThread1,
											 "randDividendThread1",
											 DividendOK_entery,
											 RT_NULL,
											 &randDividendStack[0],
											 sizeof(randDividendStack),
											 4,
											 10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&randDividendThread1);
	} else return 0;
	
	ret = rt_thread_init(&DivisionThread1,
												"DivisionThread1",
												Division_entery,
												RT_NULL,
												&divisionStack[0],
												sizeof(divisionStack),
												4,
												40);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&DivisionThread1);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(DivTaskInit);


// 示例线程
// 定义事件
#define EVENT_DIVIDEROK_FLAG12 (1<<12)
#define EVENT_DIVIDENDOK_FLAG0 (1<<0)
int divider, dividend = 0;
// 获取随机数
int getRandValue(int min, int max)
{
	srand(rt_tick_get()); 				// 初始化随机数种子
	return rand()%(max-min)+min; 	// 生成一个[t,d)区间内的整数
}
// 除数，随机生成[-20,20]区间的值，负数为不ok
void DividerOK_entery(void* parameter)
{
	while(1)
	{
		if ((divider = getRandValue(-20, 20)) > 0)  // 随机除数大于0为准备ok
		{
			rt_event_send(div_event_create, EVENT_DIVIDEROK_FLAG12);
		}
	}
}
// 被除数，随机生成[-2,2]区间的值，0为不ok
void DividendOK_entery(void* parameter)
{
	while(1)
	{
		if ((dividend = getRandValue(-1, 1)) != 0)  // 随机被除数不等于0为准备ok
		{
			rt_event_send(div_event_create, EVENT_DIVIDENDOK_FLAG0);
		}
	}
}
// 除数与被除数都OK，才可以进行除数运算
// 除数与被除数任一OK，则输出信息表明其已准备OK
void Division_entery(void* parameter)
{
	while(1)
	{
		unsigned long recvE = 0;
		rt_err_t ret = rt_event_recv(div_event_create,												// 事件集名称
                      (EVENT_DIVIDEROK_FLAG12 | EVENT_DIVIDENDOK_FLAG0),  // 关注的事件
											 RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,						// 事件触发类型为OR，并且触发后自动清零
                       1000*2,																						// 超时2S
                       &recvE);																						// 接收到的事件值
		if (ret == RT_EOK)
		{
			if (recvE & EVENT_DIVIDENDOK_FLAG0)
			{
				rt_kprintf("\r\nreceived Event Value is %x, dividend=%d", recvE & EVENT_DIVIDENDOK_FLAG0, dividend);
			}
			if (recvE & EVENT_DIVIDEROK_FLAG12)
			{
				rt_kprintf("\r\nreceived Event Value is %x, divider=%d", recvE & EVENT_DIVIDEROK_FLAG12, divider);
			}
		}
		
		ret = rt_event_recv(div_event_create,																	// 事件集名称
                      (EVENT_DIVIDEROK_FLAG12 | EVENT_DIVIDENDOK_FLAG0),  // 关注的事件
											 RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,						// 事件触发类型为AND，并且触发后自动清零
                       RT_WAITING_FOREVER,																// 必须等到两者都满足才进行除法运算
                       &recvE);																						// 接收到的事件值
		if (ret == RT_EOK)
		{
			rt_kprintf("\r\ndiv operate is %d / %d = %f", divider, dividend, (float)(divider/(dividend*1.0)));
		}
		
		rt_thread_mdelay(500);
	}
}
/*************************************事件的创建/使用示例**********************************************/


