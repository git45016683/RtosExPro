#ifndef __MYMEMPOOL_H__
#define __MYMEMPOOL_H__
#include "main.h"

/*************************************内存池的创建/使用示例**********************************************/
int mempoolTaskCreate(void);														// 动态
int mempoolTaskInit(void);															// 静态

void releaseMemoryTimer(void* parameter);  							// 定时释放内存
void requestMemoryThread_entery(void* parameter);				// 从内存池申请内存的线程
void releaseMemoryThread_entery(void* parameter);				// 释放内存的线程
/*********************************************************************************************/

#endif  // __MYMEMPOOL_H__
