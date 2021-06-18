#ifndef __MYDAEMON_H__
#define __MYDAEMON_H__
#include "main.h"

// 守护任务示例用到的线程
void TaskCreateForDaemon(void);
void DaemonTask1_entery(void* parameter);
void DaemonTask3_entery(void* parameter);

void TaskCreateStaticForDaemon(void);
void DaemonTask2_entery(void* parameter);
void Daemon_entery(void* parameter);

// 创建守护任务访问队列
void createDaemonQueue(void);

#endif  // __MYDAEMON_H__
