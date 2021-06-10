#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

int TaskCreate(void);
void Task1Thread_entery(void* parameter);

void TaskInit(void);
void Task2Thread_entery(void* parameter);

void SemExTaskCreate(void);
void SemExThread_entery(void* parameter);

#endif  // __MYTHREAD_H__
