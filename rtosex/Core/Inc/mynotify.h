#ifndef __MYNOTIFY_H__
#define __MYNOTIFY_H__
#include "main.h"

// ��Ϣ֪ͨʾ���õ����߳�
void TaskCreateForNotify(void);
void NotifyTask1_entery(void* parameter);
void NotifyTask3_entery(void* parameter);
void NotifyTask5_entery(void* parameter);

void TaskCreateStaticForNotify(void);
void NotifyTask2_entery(void* parameter);
void NotifyTask4_entery(void* parameter);
void NotifyTask6_entery(void* parameter);


#endif  // __MYNOTIFY_H__
