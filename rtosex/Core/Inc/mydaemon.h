#ifndef __MYDAEMON_H__
#define __MYDAEMON_H__
#include "main.h"

// �ػ�����ʾ���õ����߳�
void TaskCreateForDaemon(void);
void DaemonTask1_entery(void* parameter);
void DaemonTask3_entery(void* parameter);

void TaskCreateStaticForDaemon(void);
void DaemonTask2_entery(void* parameter);
void Daemon_entery(void* parameter);

// �����ػ�������ʶ���
void createDaemonQueue(void);

#endif  // __MYDAEMON_H__
