#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

// ���������߳�,ʵ�����������ĵ���
int SampleTaskCreate(void);
void AddThread1_entery(void* parameter);
void AddThread2_entery(void* parameter);

/*************************************������(��)�Ĵ���/ʹ��ʾ��**********************************************/
void MutexExTaskCreate(void);												// ��
void MutexExThread_entery(void* parameter);					// ̬

void MutexExThreadInit(void);												// ��
void MutexExThread2_entery(void* parameter);				// ̬
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
