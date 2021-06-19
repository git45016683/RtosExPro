#ifndef __MYEVENT_H__
#define __MYEVENT_H__
#include "main.h"

// 事件示例用到的线程
void TaskCreateForEvent(void);
void EventTask1_entery(void* parameter);
void EventTask3_entery(void* parameter);

void TaskCreateStaticForEvent(void);
void EventTask2_entery(void* parameter);

// 事件相关的创建
void createEvent(void);
//void createCountingSemaph(void);
//void sendBinarySemphFromISR(void);
//void sendCountingSemphFromISR(uint8_t send_type);

#endif  // __MYEVENT_H__
