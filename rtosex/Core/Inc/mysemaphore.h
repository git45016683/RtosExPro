#ifndef __MYSEMAPHORE_H__
#define __MYSEMAPHORE_H__
#include "main.h"

// 信号量示例用到的线程
void TaskCreateForSemaphore(void);
void SemaphTask1_entery(void* parameter);
void SemaphTask3_entery(void* parameter);

void TaskCreateStaticForSemaphore(void);
void SemphTask2_entery(void* parameter);

// 信号量相关的创建
void createBinarySemaph(void);
void createCountingSemaph(void);
void sendBinarySemphFromISR(void);
void sendCountingSemphFromISR(uint8_t send_type);

#endif  // __MYSEMAPHORE_H__
