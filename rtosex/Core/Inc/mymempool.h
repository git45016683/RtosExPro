#ifndef __MYMEMPOOL_H__
#define __MYMEMPOOL_H__
#include "main.h"

/*************************************�ڴ�صĴ���/ʹ��ʾ��**********************************************/
int mempoolTaskCreate(void);														// ��̬
int mempoolTaskInit(void);															// ��̬

void releaseMemoryTimer(void* parameter);  							// ��ʱ�ͷ��ڴ�
void requestMemoryThread_entery(void* parameter);				// ���ڴ�������ڴ���߳�
void releaseMemoryThread_entery(void* parameter);				// �ͷ��ڴ���߳�
/*********************************************************************************************/

#endif  // __MYMEMPOOL_H__
