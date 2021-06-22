#ifndef __MYTIMER_H__
#define __MYTIMER_H__
#include "main.h"

// 定时器示例用到的线程
void CreateForLifeTimer(void);
void LifeTimer_entery(TimerHandle_t xTimer);

void CreateStaticForTestTimer(void);
void TestTimer_entery(TimerHandle_t xTimer);


#endif  // __MYTIMER_H__
