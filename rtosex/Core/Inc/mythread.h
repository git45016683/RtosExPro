#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

/*************************************线程的创建/使用示例**********************************************/
int TaskCreate(void);															// 动 
void Task1Thread_entery(void* parameter);					// 态

void TaskInit(void);															// 静
void Task2Thread_entery(void* parameter);					// 态


/*************************************信号量的创建/使用示例**********************************************/
void SemExTaskCreate(void);												// 动
void SemExThread_entery(void* parameter);					// 态

void SemExThreadInit(void);												// 静
void SemExThread2_entery(void* parameter);				// 态
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
