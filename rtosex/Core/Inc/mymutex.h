#ifndef __MYMUTEX_H__
#define __MYMUTEX_H__
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

#endif  // __MYMUTEX_H__
