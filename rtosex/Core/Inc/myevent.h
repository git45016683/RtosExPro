#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

/*************************************�¼��Ĵ���/ʹ��ʾ��**********************************************/
int DivTaskCreate(void);																	// ��̬
int DivTaskInit(void);																		// ��̬

void DividerOK_entery(void* parameter);  									// ����OK
void DividendOK_entery(void* parameter);									// ������OK
void Division_entery(void* parameter);										// ����������Խ���
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
