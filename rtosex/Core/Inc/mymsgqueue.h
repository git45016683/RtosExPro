#ifndef __MYMSGQUEUE_H__
#define __MYMSGQUEUE_H__
#include "main.h"

/*************************************��Ϣ���еĴ���/ʹ��ʾ��**********************************************/
int msgQueueboxTaskCreate(void);												// ��̬
int msgQueueTaskInit(void);															// ��̬

void sendMsg_entery(void* parameter);  									// msg�����߳�
void recvMsg_entery(void* parameter);										// msg�����߳�
/*********************************************************************************************/

#endif  // __MYMSGQUEUE_H__
