#ifndef __MYCRITICALSUSPEND_H__
#define __MYCRITICALSUSPEND_H__
#include "main.h"

// �ٽ���/������ʾ���õ����߳�
void TaskCreateForCritical(void);
void CriticalTask1_entery(void* parameter);
void CriticalTask3_entery(void* parameter);

void TaskCreateStaticForCritical(void);
void CriticalTask2_entery(void* parameter);


#endif  // __MYCRITICALSUSPEND_H__
