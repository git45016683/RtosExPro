#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

// ���������߳�,ʵ�����������ĵ���
int SampleTaskCreate(void);
void AddThread1_entery(void* parameter);
void AddThread2_entery(void* parameter);

/*************************************������(��)�Ĵ���/ʹ��ʾ��**********************************************/
int SampleWithMutexTaskCreate(void);												// ��̬
int SampleWithMutexTaskInit(void);												// ��̬

void AddThread11_entery(void* parameter);
void AddThread22_entery(void* parameter);
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
