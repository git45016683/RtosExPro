#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "main.h"

/*************************************事件的创建/使用示例**********************************************/
int DivTaskCreate(void);																	// 动态
int DivTaskInit(void);																		// 静态

void DividerOK_entery(void* parameter);  									// 除数OK
void DividendOK_entery(void* parameter);									// 被除数OK
void Division_entery(void* parameter);										// 除法运算可以进行
/*********************************************************************************************/

#endif  // __MYTHREAD_H__
