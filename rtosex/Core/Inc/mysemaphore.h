#ifndef __MYSEMAPHORE_H__
#define __MYSEMAPHORE_H__
#include "main.h"

// �ź���ʾ���õ����߳�
void TaskCreateForSemaphore(void);
void SemaphTask1_entery(void* parameter);
void SemaphTask3_entery(void* parameter);

void TaskCreateStaticForSemaphore(void);
void SemphTask2_entery(void* parameter);

// �ź�����صĴ���
void createBinarySemaph(void);
void createCountingSemaph(void);
void sendBinarySemphFromISR(void);
void sendCountingSemphFromISR(uint8_t send_type);

#endif  // __MYSEMAPHORE_H__
