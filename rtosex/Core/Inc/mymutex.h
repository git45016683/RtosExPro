#ifndef __MYMUTEX_H__
#define __MYMUTEX_H__
#include "main.h"

// 创建两个线程,实现两个变量的递增
int SampleTaskCreate(void);
void AddThread1_entery(void* parameter);
void AddThread2_entery(void* parameter);

/*************************************互斥量(锁)的创建/使用示例**********************************************/
int SampleWithMutexTaskCreate(void);												// 动态
int SampleWithMutexTaskInit(void);												// 静态

void AddThread11_entery(void* parameter);
void AddThread22_entery(void* parameter);
/*********************************************************************************************/

#endif  // __MYMUTEX_H__
