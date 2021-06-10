#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

// 创建两个线程,实现两个变量的递增
int SampleTaskCreate(void);
void AddThread1_entery(void* parameter);
void AddThread2_entery(void* parameter);

/*************************************互斥量(锁)的创建/使用示例**********************************************/
void MutexExTaskCreate(void);												// 动
void MutexExThread_entery(void* parameter);					// 态

void MutexExThreadInit(void);												// 静
void MutexExThread2_entery(void* parameter);				// 态
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
