#ifndef __MYMSGQUEUE_H__
#define __MYMSGQUEUE_H__
#include "main.h"

// ��Ϣ����ʾ���õ����߳�
void TaskCreateForQueue(void);
void QueueTask1_entery(void* parameter);

void TaskCreateStaticForQueue(void);
void QueueTask2_entery(void* parameter);

// ��Ϣ������صĴ���
void createMyQueue(void);
void deleteMyQueue(void);
void sendMyQueueFromISR(uint8_t send_type);

#endif  // __MYMSGQUEUE_H__
