#ifndef __MYCRITICALSUSPEND_H__
#define __MYCRITICALSUSPEND_H__
#include "main.h"

// 临界区/调度器示例用到的线程
void TaskCreateForCritical(void);
void CriticalTask1_entery(void* parameter);
void CriticalTask3_entery(void* parameter);

void TaskCreateStaticForCritical(void);
void CriticalTask2_entery(void* parameter);


// 互斥量(信号量)相关的创建
void createMutex(void);

#endif  // __MYCRITICALSUSPEND_H__
