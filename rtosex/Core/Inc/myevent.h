#ifndef __MYEVENT_H__
#define __MYEVENT_H__
#include "main.h"

// �¼�ʾ���õ����߳�
void TaskCreateForEvent(void);
void EventTask1_entery(void* parameter);
void EventTask3_entery(void* parameter);

void TaskCreateStaticForEvent(void);
void EventTask2_entery(void* parameter);

// �¼���صĴ���
void createEvent(void);
//void createCountingSemaph(void);
//void sendBinarySemphFromISR(void);
//void sendCountingSemphFromISR(uint8_t send_type);

#endif  // __MYEVENT_H__
