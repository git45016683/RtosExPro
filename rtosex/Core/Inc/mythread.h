#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

/*************************************�̵߳Ĵ���/ʹ��ʾ��**********************************************/
int TaskCreate(void);															// �� 
void Task1Thread_entery(void* parameter);					// ̬

void TaskInit(void);															// ��
void Task2Thread_entery(void* parameter);					// ̬


/*************************************�ź����Ĵ���/ʹ��ʾ��**********************************************/
void SemExTaskCreate(void);												// ��
void SemExThread_entery(void* parameter);					// ̬

void SemExThreadInit(void);												// ��
void SemExThread2_entery(void* parameter);				// ̬
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
