#ifndef __MYTIMER_H__
#define __MYTIMER_H__
#include "main.h"

/*************************************定时器的创建/使用示例**********************************************/
int timerTaskCreate(void);														// 动态
int timerTaskInit(void);															// 静态

void lifeLogTimerOut(void* parameter);  							// 定时生命周期信息log输出
void testTimerOut(void* parameter);										// 测试定时器函数
/*********************************************************************************************/

#endif  // __MYTIMER_H__
