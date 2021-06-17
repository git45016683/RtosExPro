#ifndef __MYMSGQUEUE_H__
#define __MYMSGQUEUE_H__
#include "main.h"

// 消息队列示例用到的线程
void TaskCreateForQueue(void);
void QueueTask1_entery(void* parameter);

void TaskCreateStaticForQueue(void);
void QueueTask2_entery(void* parameter);

// 消息队列相关的创建
void createMyQueue(void);
void deleteMyQueue(void);
void sendMyQueueFromISR(uint8_t send_type);

#endif  // __MYMSGQUEUE_H__
