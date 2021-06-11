#ifndef __MYMSGQUEUE_H__
#define __MYMSGQUEUE_H__
#include "main.h"

/*************************************消息队列的创建/使用示例**********************************************/
int msgQueueboxTaskCreate(void);												// 动态
int msgQueueTaskInit(void);															// 静态

void sendMsg_entery(void* parameter);  									// msg发送线程
void recvMsg_entery(void* parameter);										// msg接收线程
/*********************************************************************************************/

#endif  // __MYMSGQUEUE_H__
