#ifndef __MYTIMER_H__
#define __MYTIMER_H__
#include "main.h"

/*************************************��ʱ���Ĵ���/ʹ��ʾ��**********************************************/
int timerTaskCreate(void);														// ��̬
int timerTaskInit(void);															// ��̬

void lifeLogTimerOut(void* parameter);  							// ��ʱ����������Ϣlog���
void testTimerOut(void* parameter);										// ���Զ�ʱ������
/*********************************************************************************************/

#endif  // __MYTIMER_H__
