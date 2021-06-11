#include "mymailbox.h"
#include <stdlib.h>
#include <string.h>

/*************************************邮箱的创建/使用示例**********************************************/
// 声明邮箱控制块指针
rt_mailbox_t samplemail_create = RT_NULL;
rt_thread_t sendmailThread;  // mail发送线程
rt_thread_t recvmailThread;  // mail接收线程
int sampleMailboxTaskCreate(void)
{
	samplemail_create = rt_mb_create("samplemail_create",  // 邮箱名
																	 128, 								 // 邮箱大小
																	 RT_IPC_FLAG_FIFO);		 // 邮箱取出的顺序-先进先出
	if (samplemail_create != RT_NULL)
	{
		rt_kprintf("\r\ncreate samplemail_create success");
	}
	
	sendmailThread = rt_thread_create("sendmailThread",
                             sendMail_entery,
                             RT_NULL,
                             256,
                             4,
                             10);
	if (sendmailThread != RT_NULL)
	{
		rt_thread_startup(sendmailThread);
	} else return 0;
	
	recvmailThread = rt_thread_create("recvmailThread",
																		recvMail_entery,
																		RT_NULL,
																		256,
																		4,
																		40);
	if (recvmailThread != RT_NULL)
	{
		rt_thread_startup(recvmailThread);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(sampleMailboxTaskCreate);

//------------------------------------------------------------------------------------
// 静态创建事件
struct rt_mailbox samplemail_init;
char mailbox[256] = {0x00};
struct rt_thread sendmailThread1;
static char sendmailStack[256];
struct rt_thread recvmailThread1;
static char recvmailStack[256];
int sampleMailboxTaskInit(void)
{
	rt_err_t ret = rt_mb_init(&samplemail_init,
														"samplemail_init",
														&mailbox[0],
														sizeof(mailbox),
														RT_IPC_FLAG_FIFO);
	if (ret == RT_EOK)
	{
		rt_kprintf("\r\nsamplemail_init init success");
	} else return 0;
	
	ret = rt_thread_init(&sendmailThread1,
											 "sendmailThread1",
											 sendMail_entery,
											 RT_NULL,
											 &sendmailStack[0],
											 sizeof(sendmailStack),
											 4,
											 10);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&sendmailThread1);
	} else return 0;
	
	ret = rt_thread_init(&recvmailThread1,
												"recvmailThread1",
												recvMail_entery,
												RT_NULL,
												&recvmailStack[0],
												sizeof(recvmailStack),
												4,
												40);
	if (ret == RT_EOK)
	{
		rt_thread_startup(&recvmailThread1);
		return 1;
	}
	return 0;
}
//INIT_APP_EXPORT(sampleMailboxTaskInit);


// 示例线程
void errinfo(int index, rt_err_t err)
{
	switch(err)
	{
		case -RT_ETIMEOUT:
			rt_kprintf("\r\n%d-errNo=%d, it means timeout", index, err);
			break;
		case -RT_EFULL:
			rt_kprintf("\r\n%d-errNo=%d, it means mailbox full", index, err);
			break;
		default:
			rt_kprintf("\r\n%d-errNo=%d", index, err);
	}
}
void sendMail_entery(void* parameter)
{
	char mail1[] = "wo wang ji le wo shi shui, ";
	char mail2[] = "ni neng gao su wo ma? ";
//	char mail3[] = "SOS !!! SOS !!! ";
	char mailtemp[32] = "";
	uint32_t sendCount = 0;
	char count[8] = "";
	rt_err_t ret = RT_EOK;
	while(1)
	{
		sprintf(count, "%d", sendCount);
		if (0 == (sendCount%2))
			memcpy(mailtemp, mail1, sizeof(mail1));
		else
			memcpy(mailtemp, mail2, sizeof(mail2));
		strncat(mailtemp, count, 10);
		
		#if 0  // 动态
		ret = rt_mb_send(samplemail_create, (rt_uint32_t)&mailtemp);
//		ret = rt_mb_send_wait(samplemail_create, (rt_uint32_t)&mailtemp, 100);
//		rt_mb_urgent(samplemail_create, (unsigned long)&mailtemp);  // nano版没有发送紧急邮件接口
		#else
//		ret = rt_mb_send(&samplemail_init, (rt_uint32_t)&mailtemp);
		ret = rt_mb_send_wait(&samplemail_init, (rt_uint32_t)&mailtemp, 100);
//		rt_mb_urgent(&samplemail_init, (unsigned long)&mailtemp);  // nano版没有发送紧急邮件接口
		#endif
		if (ret != RT_EOK) errinfo(1, ret);
		
		rt_thread_mdelay(3000);
		sendCount++;
	}
}

void recvMail_entery(void* parameter)
{
	char* mail = "";
	rt_err_t ret = RT_EOK;
	while(1)
	{
		#if 0  // 动态
		ret = rt_mb_recv(samplemail_create,
										(unsigned long*)&mail,
										1000);
		#else
		ret = rt_mb_recv(&samplemail_init,
										(unsigned long*)&mail,
										1000);
		#endif
		if (ret == RT_EOK)
		{
			rt_kprintf("\r\n1.receive a mail: %s", mail);
		} else errinfo(2, ret);

		#if 0  // 动态
		ret = rt_mb_recv(samplemail_create,
										(unsigned long*)&mail,
										RT_WAITING_FOREVER);
		#else
		ret = rt_mb_recv(&samplemail_init,
										(rt_uint32_t*)&mail,
										RT_WAITING_FOREVER);
		#endif
		if (ret == RT_EOK)
		{
			rt_kprintf("\r\n2.receive a mail: %s", mail);
		} else errinfo(3, ret);
	}
}
/*************************************邮箱的创建/使用示例**********************************************/


