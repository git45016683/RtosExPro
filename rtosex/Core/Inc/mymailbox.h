#ifndef __MYMAILBOX_H__
#define __MYMAILBOX_H__
#include "main.h"

/*************************************�¼��Ĵ���/ʹ��ʾ��**********************************************/
int sampleMailboxTaskCreate(void);												// ��̬
int sampleMailboxTaskInit(void);													// ��̬

void sendMail_entery(void* parameter);  									// mail�����߳�
void recvMail_entery(void* parameter);										// mail�����߳�
/*********************************************************************************************/

#endif  // __MYMAILBOX_H__
