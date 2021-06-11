#ifndef __MYMAILBOX_H__
#define __MYMAILBOX_H__
#include "main.h"

/*************************************事件的创建/使用示例**********************************************/
int sampleMailboxTaskCreate(void);												// 动态
int sampleMailboxTaskInit(void);													// 静态

void sendMail_entery(void* parameter);  									// mail发送线程
void recvMail_entery(void* parameter);										// mail接收线程
/*********************************************************************************************/

#endif  // __MYMAILBOX_H__
